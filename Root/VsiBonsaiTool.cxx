#include <algorithm>
#include <numeric>

#include <TVector3.h>
#include <TLorentzVector.h>
#include <TMath.h>

#include "EJsAnalysis/VsiBonsaiTool.h"

namespace VsiBonsai {

  MSG::Level outputLevel = MSG::INFO;

  //_____________________________________________________________________________________________
  void setDefaultConfig ( Configurator& config ) {
    config.clear();
    config[ Config::hitPatternCondition ] = VsiTool::HitPatternCondition::NONE;
    config[ Config::d0_wrtSVCut         ] = AlgConsts::invalidFloat;
    config[ Config::z0_wrtSVCut         ] = AlgConsts::invalidFloat;
    config[ Config::errd0_wrtSVCut      ] = AlgConsts::invalidFloat;
    config[ Config::errz0_wrtSVCut      ] = AlgConsts::invalidFloat;
    config[ Config::d0signif_wrtSVCut   ] = AlgConsts::invalidFloat;
    config[ Config::z0signif_wrtSVCut   ] = AlgConsts::invalidFloat;
    config[ Config::chi2_toSVCut        ] = AlgConsts::invalidFloat;
    return;
  }

  
  //_____________________________________________________________________________________________
  void setOutputLevel ( MSG::Level level ) { outputLevel = level; }

  
#define ERRMSG_UNCONFIG( NAME ) \
  std::cerr << "ERROR" << __PRETTY_FUNCTION__ << ": unconfigured (" << #NAME << ")" << std::endl;
#define ERRMSG_TYPE( NAME, TYPE ) \
  std::cerr << "ERROR" << __PRETTY_FUNCTION__ << ": type error (" << #NAME << ", "; \
  std::cerr << demangle( typeid( TYPE ).name() ) << ")" << std::endl;

  
  //_____________________________________________________________________________________________
  bool chi2Filter ( const xAOD::Vertex*, const xAOD::TrackParticle* trk,
		    const xAOD::Vertex*, const Configurator& config )
  {
    if ( config.find( Config::trackChi2Cut ) == config.end() ) {
      ERRMSG_UNCONFIG( Config::trackChi2Cut );
      throw( Error::Unconfigured );
    }

    if ( config.at( Config::trackChi2Cut ).type() != typeid( double ) ) {
      ERRMSG_TYPE( Config::trackChi2Cut, double );
      throw( Error::TypeError );
    }

    const double threshold = config.at( Config::trackChi2Cut ).getVar<double>();

    return ( trk->chiSquared() / trk->numberDoF() ) < threshold;
  }

  
  //_____________________________________________________________________________________________
  bool hitPatternFilter ( const xAOD::Vertex* dv, const xAOD::TrackParticle* trk,
			  const xAOD::Vertex* pv, const Configurator& config )
  {
    if ( config.find( Config::hitPatternCondition ) == config.end() ) {
      ERRMSG_UNCONFIG( Config::hitPatternCondition );
      throw( Error::Unconfigured );
    }

    if ( config.at( Config::hitPatternCondition ).type() !=
	 typeid( VsiTool::HitPatternCondition ) ) {
      ERRMSG_TYPE( Config::hitPatternCondition, VsiTool::HitPatternCondition );
      throw( Error::TypeError );
    }

    const auto& region     = VsiTool::vertexRegion( dv );
    const auto condition   =
      config.at( Config::hitPatternCondition ).getVar<VsiTool::HitPatternCondition>();
    const auto trackRelDir = VsiTool::trackRelativeDirection( trk, dv, pv );

    bool flag = true;

    switch( condition ) {
    case VsiTool::HitPatternCondition::LOOSE:
      flag = VsiTool::passLoosePattern  ( trk->hitPattern(), region, trackRelDir ); break;
    case VsiTool::HitPatternCondition::MEDIUM:
      flag = VsiTool::passMediumPattern ( trk->hitPattern(), region, trackRelDir ); break;
    case VsiTool::HitPatternCondition::TIGHT:
      flag = VsiTool::passTightPattern  ( trk->hitPattern(), region, trackRelDir ); break;
    default:
      flag = true;
    }

    return flag;
  }


  //_____________________________________________________________________________________________
  bool ipWrtSVFilter ( const xAOD::Vertex*, const xAOD::TrackParticle* trk,
		       const xAOD::Vertex*, const Configurator& config )
  {
    std::map< Config, std::string > enums {
      { Config::d0_wrtSVCut,       "Config::d0wrtSVCut"        },
      { Config::z0_wrtSVCut,       "Config::z0wrtSVCut"        },
      { Config::errd0_wrtSVCut,    "Config::errd0_wrtSVCut"    },
      { Config::errz0_wrtSVCut,    "Config::errz0_wrtSVCut"    },
      { Config::d0signif_wrtSVCut, "Config::d0signif_wrtSVCut" },
      { Config::z0signif_wrtSVCut, "Config::z0signif_wrtSVCut" }
    };

    for ( const auto& pair : enums ) {
      if ( config.find( pair.first ) == config.end() ) {
	ERRMSG_UNCONFIG( pair.second );
	throw( Error::Unconfigured );
      }

      if ( config.at( pair.first ).type() != typeid( double ) ) {
	ERRMSG_TYPE( pair.second, double );
	throw( Error::TypeError );
      }
    }

    ////////////
    // lambda //
    auto ipsignif_wrtSVCut = [&]( const xAOD::TrackParticle* trk,
				  const enum Config& cfgname ) -> bool {
      const auto& cutval = config.at( cfgname ).getVar<double>();
      if ( AlgConsts::invalidFloat == cutval ) return true;
      if ( cutval < 0. )                       return true;

      std::string name;
      if      ( cfgname == Config::d0signif_wrtSVCut ) name = "d0";
      else if ( cfgname == Config::z0signif_wrtSVCut ) name = "z0";
      else {
	std::cerr << "ERROR: " << __FUNCTION__ << ": invalid config type!" << std::endl;
	throw( Error::TypeError );
      }

      // find variables
      if ( !trk->isAvailable<float>( name + "_wrtSV" ) ) {
	std::cerr << "WARNING: " << __FUNCTION__ << ": auxdyn variable ";
	std::cerr << ( name + "_wrtSV" ) << " not available!" << std::endl;
	return true;
      }
      if ( !trk->isAvailable<float>( "err" + name + "_wrtSV" ) ) {
	std::cerr << "WARNING: " << __FUNCTION__ << ": auxdyn variable ";
	std::cerr << ( "err" + name + "_wrtSV" ) << " not available! " << std::endl;
	return true;
      }

      auto& ip_wrtSV    = trk->auxdataConst<float>(         name + "_wrtSV" );
      auto& iperr_wrtSV = trk->auxdataConst<float>( "err" + name + "_wrtSV" );
      const auto ipsignif_wrtSV = fabs( ip_wrtSV ) / sqrt( fabs( iperr_wrtSV ) );

      if ( fabs( ipsignif_wrtSV ) > cutval ) return false;

      return true;
    };
    ////////////

    if ( !varWrtSVCut<Config::d0_wrtSVCut>    ( trk, config, "d0_wrtSV"        ) ) return false;
    if ( !varWrtSVCut<Config::z0_wrtSVCut>    ( trk, config, "z0_wrtSV"        ) ) return false;
    if ( !varWrtSVCut<Config::errd0_wrtSVCut> ( trk, config, "errd0_wrtSV"     ) ) return false;
    if ( !varWrtSVCut<Config::errz0_wrtSVCut> ( trk, config, "errz0_wrtSV"     ) ) return false;
    if ( !ipsignif_wrtSVCut                   ( trk, Config::d0signif_wrtSVCut ) ) return false;
    if ( !ipsignif_wrtSVCut                   ( trk, Config::z0signif_wrtSVCut ) ) return false;

    return true;
  }


  //_____________________________________________________________________________________________
  bool dropAssociated ( const xAOD::Vertex*, const xAOD::TrackParticle* trk,
			const xAOD::Vertex*, const Configurator& config )
  {
    if ( config.find( Config::dropAssociated ) == config.end() ) {
      ERRMSG_UNCONFIG( Config::dropAssociated );
      throw( Error::Unconfigured );
    }

    if ( config.at( Config::dropAssociated ).type() != typeid( bool ) ) {
      ERRMSG_TYPE( Config::dropAssociated, bool );
      throw( Error::TypeError );
    }

    if ( !config.at( Config::dropAssociated ).getVar<bool>() ) return true;
    if ( !trk->isAvailable<char>( "is_associated" ) )          return true;
    
    auto is_assoc = trk->auxdataConst<char>( "is_associated" );
    if ( is_assoc ) return false;

    return true;
  }


  //_____________________________________________________________________________________________
  bool dropNonSelected( const xAOD::Vertex*, const xAOD::TrackParticle* trk,
			const xAOD::Vertex*, const Configurator& config )
  {
    if ( config.find( Config::dropNonSelected ) == config.end() ) {
      ERRMSG_UNCONFIG( Config::dropNonSelected );
      throw( Error::Unconfigured );
    }

    if ( config.at( Config::dropNonSelected ).type() != typeid( bool ) ) {
      ERRMSG_TYPE( Config::dropNonSelected, bool );
      throw( Error::TypeError );
    }

    if ( !config.at( Config::dropNonSelected ).getVar<bool>() ) return true;
    if ( !trk->isAvailable<char>( "is_selected" ) )             return true;

    auto is_sel = trk->auxdataConst<char>( "is_selected" );
    if ( !is_sel ) return false; // is this right ??? 

    return true;
  }


  #undef ERRMSG_UNCONFIG
  #undef ERRMSG_TYPE


#define AUXDYN( obj, type, varname ) \
  ( obj->isAvailable<type>(varname) ? obj->auxdataConst<type>(varname) : AlgConsts::invalidFloat )

  
  //_____________________________________________________________________________________________
  TLorentzVector sumP4 ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    TLorentzVector v;
    for ( const auto& trk : tracks ) {
      if ( !trk ) continue;
      TLorentzVector vv;
      vv.SetPtEtaPhiM( AUXDYN( trk, float, "pt_wrtSV"  ), AUXDYN( trk, float, "eta_wrtSV" ),
		       AUXDYN( trk, float, "phi_wrtSV" ), PhysConsts::mass_chargedPion    );
      if ( !trk->isAvailable<float>( "pt_wrtSV" ) )
	vv.SetPtEtaPhiM( trk->pt(), trk->eta(), trk->phi(), PhysConsts::mass_chargedPion );
      v += vv;
    }
    return v;
  }

  
  //_____________________________________________________________________________________________
  TLorentzVector sumP4 ( const std::vector< TLorentzVector >& tracks ) {
    TLorentzVector sum;
    for ( auto& trk : tracks ) sum += trk;
    return sum;
  }

  
  //_____________________________________________________________________________________________
  double scalarSumP ( const std::vector< const xAOD::TrackParticle* >& tracks ) {
    double H = 0.0;
    for ( const auto& trk : tracks ) {
      if ( !trk ) continue;
      TLorentzVector vv;
      vv.SetPtEtaPhiM( AUXDYN( trk, float, "pt_wrtSV"  ), AUXDYN( trk, float, "eta_wrtSV" ),
		       AUXDYN( trk, float, "phi_wrtSV" ), PhysConsts::mass_chargedPion    );
      if ( !trk->isAvailable<float>( "pt_wrtSV" ) )
	vv.SetPtEtaPhiM( trk->pt(), trk->eta(), trk->phi(), PhysConsts::mass_chargedPion );
      H += vv.Vect().Mag();
    }
    return H;
  }

  
  //_____________________________________________________________________________________________
  double scalarSumPt ( const std::vector< const xAOD::TrackParticle* >& tracks ) {
    double Ht = 0.0;
    for ( const auto& trk : tracks ) {
      if ( !trk ) continue;
      TLorentzVector vv;
      vv.SetPtEtaPhiM( AUXDYN( trk, float, "pt_wrtSV"  ), AUXDYN( trk, float, "eta_wrtSV" ),
		       AUXDYN( trk, float, "phi_wrtSV" ), PhysConsts::mass_chargedPion    );
      if ( !trk->isAvailable<float>( "pt_wrtSV" ) )
	vv.SetPtEtaPhiM( trk->pt(), trk->eta(), trk->phi(), PhysConsts::mass_chargedPion );
      Ht += vv.Pt();
    }
    return Ht;
  }

  
  //_____________________________________________________________________________________________
  int sumCharge ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    int charge = AlgConsts::invalidInt;
    for ( const auto& trk : tracks ) {
      if ( !trk ) continue;
      charge += trk->charge();
    }
    return charge;
  }

  
  //_____________________________________________________________________________________________
  double minOpAng ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    double opAng = -1.0 * AlgConsts::maxValue;
    for ( auto i_itr = tracks.begin(); i_itr != tracks.end(); ++i_itr ) {
      TVector3 iv;
      iv.SetPtEtaPhi( 1.0, AUXDYN( (*i_itr), float, "eta_wrtSV" ),
		           AUXDYN( (*i_itr), float, "phi_wrtSV" ) );
      for ( auto j_itr = std::next( i_itr ); j_itr != tracks.end(); ++j_itr ) {
	TVector3 jv;
	jv.SetPtEtaPhi( 1.0, AUXDYN( (*j_itr), float, "eta_wrtSV" ),
			     AUXDYN( (*j_itr), float, "phi_wrtSV" ) );
	double tmp = iv * jv / ( iv.Mag() * jv.Mag() );
	if ( opAng < tmp ) opAng = tmp;
      }
    }
    return opAng;
  }

  
  //_____________________________________________________________________________________________
  double maxOpAng ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    double opAng = AlgConsts::maxValue;
    for ( auto i_itr = tracks.begin(); i_itr != tracks.end(); ++i_itr ) {
      TVector3 iv;
      iv.SetPtEtaPhi( 1.0, AUXDYN( (*i_itr), float, "eta_wrtSV" ),
		           AUXDYN( (*i_itr), float, "phi_wrtSV" ) );
      for ( auto j_itr = std::next( i_itr ); j_itr != tracks.end(); ++j_itr ) {
	TVector3 jv;
	jv.SetPtEtaPhi( 1.0, AUXDYN( (*j_itr), float, "eta_wrtSV" ),
			     AUXDYN( (*j_itr), float, "phi_wrtSV" ) );
	double tmp = iv * jv / ( iv.Mag() * jv.Mag() );
	if ( opAng > tmp ) opAng = tmp;
      }
    }
    return opAng;
  }

  
  //_____________________________________________________________________________________________
  double mind0 ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    auto min = std::min_element( tracks.begin(), tracks.end(),
				 []( const auto* trk1, const auto* trk2 )
				 { return fabs( trk1->d0() ) < fabs( trk2->d0() ); } );
    return fabs( (*min)->d0() );
  }

  
  //_____________________________________________________________________________________________
  double maxd0 ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    auto max = std::max_element( tracks.begin(), tracks.end(),
				 []( const auto* trk1, const auto* trk2 )
				 { return fabs( trk1->d0() ) < fabs( trk2->d0() ); } );
    return fabs( (*max)->d0() );
  }

  
  //_____________________________________________________________________________________________
  double minOneTrackRemovedMass ( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    std::vector<double> masses;
    for ( const auto& trk : tracks ) {
      auto wrkTrks = tracks;
      wrkTrks.erase( std::remove_if( wrkTrks.begin(), wrkTrks.end(),
				     [trk]( const xAOD::TrackParticle* t )
				     { return trk == t; } ), wrkTrks.end() );    
      auto vv = std::accumulate( wrkTrks.begin(), wrkTrks.end(), TLorentzVector{},
				 []( TLorentzVector& v, const xAOD::TrackParticle* trk )
				 { TLorentzVector tmp;
				   tmp.SetPtEtaPhiM( AUXDYN( trk, float, "pt_wrtSV"  ),
						     AUXDYN( trk, float, "eta_wrtSV" ),
						     AUXDYN( trk, float, "phi_wrtSV" ),
						     PhysConsts::mass_chargedPion    );
				   v += tmp; return v; } );   
      masses.emplace_back( vv.M() );
    }
    auto min = std::min_element( masses.begin(), masses.end() );
    return *min;
  }


  //_____________________________________________________________________________________________
  std::vector< std::tuple< double, double, std::pair< const xAOD::TrackParticle*,
						      const xAOD::TrackParticle* > > >
  allTwoTracksMassCombinations( const std::vector< const xAOD::TrackParticle* >& tracks )
  {
    using Tuple = std::tuple< double, double, std::pair< const xAOD::TrackParticle*,
							 const xAOD::TrackParticle* > >;
    std::vector< Tuple > container;
    for ( auto i_itr = tracks.begin(); i_itr != tracks.end(); ++i_itr ) {
      for ( auto j_itr = std::next( i_itr ); j_itr != tracks.end(); ++j_itr ) {
	TLorentzVector v4_2trk;
	TLorentzVector v4_rest;
	for ( const auto& trk : tracks ) {
	  TLorentzVector trk_p4;
	  trk_p4.SetPtEtaPhiM( AUXDYN( trk, float, "pt_wrtSV"  ),
			       AUXDYN( trk, float, "eta_wrtSV" ),
			       AUXDYN( trk, float, "phi_wrtSV" ),
			       PhysConsts::mass_chargedPion    );	  
	  if ( ( trk == (*i_itr) ) || ( trk == (*j_itr) ) ) v4_2trk += trk_p4;
	  else                                              v4_rest += trk_p4;
	}	
	auto tuple =
	  std::make_tuple( v4_2trk.M(), v4_rest.M(), std::make_pair( (*i_itr), (*j_itr) ) );
	container.emplace_back( tuple );	
      }
    }
    return container;
  }


  //_____________________________________________________________________________________________
  std::vector< std::vector< TLorentzVector > >
  kTalg ( const std::vector< const xAOD::TrackParticle* >& tracks,
	  const double& R, const double& power, const double& threshold )
  {
    using namespace asg::msgUserCode;

    std::vector< std::vector< TLorentzVector > > clusters;
    for ( const auto& trk : tracks ) {
      TLorentzVector v;
      v.SetPtEtaPhiM( AUXDYN( trk, float, "pt_wrtSV"  ), AUXDYN( trk, float, "eta_wrtSV" ),
		      AUXDYN( trk, float, "phi_wrtsV" ), PhysConsts::mass_chargedPion    );
      if ( !trk->isAvailable<float>( "pt_wrtSV" ) )
	v.SetPtEtaPhiM( trk->pt(), trk->eta(), trk->phi(), PhysConsts::mass_chargedPion );
      clusters.emplace_back( std::vector< TLorentzVector >( 1, v ) );
    }

    std::vector< std::vector< TLorentzVector > > jets;
    std::map< std::pair< unsigned, unsigned >, double > dlist;
    std::map< unsigned, double > blist;
    for ( size_t i = 0; i != clusters.size(); ++i ) {
      for ( size_t j = 0; j != clusters.size(); ++j ) {
	
	if ( i > j ) continue;
	
	auto isum = sumP4( clusters.at(i) );
	auto jsum = sumP4( clusters.at(j) );

	if ( i == j )
	  blist[i] = pow( isum.Pt(), power );
	else if ( i < j ) {
	  auto deta    = isum.Eta() - jsum.Eta();
	  auto dphi    = VsiTool::dPhiMod( isum.Phi(), jsum.Phi() );
	  auto dRrelSq = ( deta*deta + dphi*dphi ) / ( R*R );
	  auto pair    = std::make_pair( i, j );
	  auto dist    = std::min( pow( isum.Pt(), power ), pow( jsum.Pt(), power ) );
	  dlist.emplace( std::make_pair( pair, dist ) );
	}
      }
    }

    while ( blist.size() > 0 ) {
      if ( blist.size() == 1 ) {
	auto itr = blist.begin();
	jets.emplace_back( clusters.at( (*itr).first ) );
	blist.erase( blist.begin() );
	continue;
      }

      auto min_dij =
	std::min_element( dlist.begin(), dlist.end(),
			  []( std::pair< std::pair< unsigned, unsigned >, double > p1,
			      std::pair< std::pair< unsigned, unsigned >, double > p2 )
			  { return p1.second < p2.second; } );
      auto min_bi =
	std::min_element( blist.begin(), blist.end(),
			  []( std::pair< unsigned, double > p1,
			      std::pair< unsigned, double > p2 )
			  { return p1.second < p2.second; } );

      //ANA_MSG_INFO( __FUNCTION__ << ": min_dij = " << (*min_dij).second
      //	                 << ", min_bi = " << (*min_bi).second );

      if ( (*min_dij).second < (*min_bi).second ) {
	auto& i = (*min_dij).first.first;
	auto& j = (*min_dij).first.second;

	//ANA_MSG_INFO( __FUNCTION__ << ": merging " << i << " and " << j );

	for ( auto& v : clusters.at(j) ) { clusters.at(i).emplace_back( v ); }
	clusters.at(j).clear();

	auto isum = sumP4( clusters.at(i) );
	blist.at(i) = pow( isum.Pt(), power );

	for ( auto& pair : dlist ) {
	  if ( pair.first.first == i ) {
	    auto isum = sumP4( clusters.at(i) );
	    auto ksum = sumP4( clusters.at( pair.first.second ) );
	    auto dR = hypot( isum.Eta() - ksum.Eta(), VsiTool::dPhiMod(isum.Phi(), ksum.Phi() ) );
	    pair.second = std::min( pow( isum.Pt(), power ), pow( ksum.Pt(), power ) ) * dR/R;
	  }
	  if ( pair.first.second == i ) {
	    auto isum = sumP4( clusters.at(i) );
	    auto ksum = sumP4( clusters.at( pair.first.first ) );
	    auto dR = hypot( isum.Eta() - ksum.Eta(), VsiTool::dPhiMod( isum.Phi(), ksum.Phi() ) );
	    pair.second = std::min( pow( isum.Pt(), power ), pow( ksum.Pt(), power ) ) * dR/R;
	  }
	}
	for ( auto itr = dlist.begin(); itr != dlist.end(); ) {
	  if ( (*itr).first.first == j || (*itr).first.second == j ) dlist.erase( itr++ );
	  else ++itr;
	}
	for ( auto itr = blist.begin(); itr != blist.end(); ) {
	  if ( (*itr).first == j ) blist.erase( itr++ );
	  else ++itr;
	}
      }
      else {
	//ANA_MSG_INFO( __FUNCTION__ << ": found a jet" );
	auto& i = (*min_bi).first;
	jets.emplace_back( clusters.at(i) );

	for ( auto itr = dlist.begin(); itr != dlist.end(); ) {
	  if ( (*itr).first.first == i || (*itr).first.second == i ) dlist.erase( itr++ );
	  else ++itr;
	}
	for ( auto itr = blist.begin(); itr != blist.end(); ) {
	  if ( (*itr).first == i ) blist.erase( itr++ );
	  else ++itr;
	}
      }
      //ANA_MSG_INFO( __FUNCTION__ << ": --> jet size = "      << jets.size()
      //	                 << ", input tracks size = " << tracks.size()
      //	                 << ", dlist size = "        << dlist.size()
      //	                 << ", blist size = "        << blist.size() );
    }

    jets.erase( std::remove_if( jets.begin(), jets.end(),
				[&]( std::vector< TLorentzVector >& j )
				{ auto sumP = sumP4( j );
				  return ( sumP.Pt() < threshold ); } ), jets.end() );
    return jets;
  }


  #undef AUXDYN

  
} // VsiBonsai




namespace VsiTool {
   
  //_____________________________________________________________________________________________
  const TVector3 dvDirection ( const xAOD::Vertex* dv, const xAOD::Vertex* pv )
  {
    auto diff = dv->position() - pv->position();
    return TVector3( diff.x(), diff.y(), diff.z() );
  }


  //_____________________________________________________________________________________________
  const TVector3 trackRelativeDirection ( const xAOD::TrackParticle* trk,
					  const xAOD::Vertex* dv, const xAOD::Vertex* pv )
  {
    const TVector3& dvDir = dvDirection( dv, pv );
    const auto& dphi = dPhiMod( trk->phi(), dvDir.Phi() );
    const auto& deta = trk->eta() - dvDir.Eta();
    TVector3 dir;
    dir.SetPtEtaPhi( 1, deta, dphi );
    return dir;
  }


  //_____________________________________________________________________________________________
  double dPhiMod ( const double& phi1, const double& phi2 )
  {
    double dphi = phi1 - phi2;
    while ( dphi >  TMath::Pi() ) dphi -= TMath::TwoPi();
    while ( dphi < -TMath::Pi() ) dphi += TMath::TwoPi();
    return dphi;
  }


  //_____________________________________________________________________________________________
  enum VertexRegion vertexRegion ( const xAOD::Vertex* dv )
  {
    // mutually exclusive vertex position pattern
    VertexRegion area;
    const double rad  = dv->position().perp();
    const double absz = fabs( dv->z() );

    if      ( rad < 23.50                 )
      area = VertexRegion::insideBeamPipe;
    else if ( rad < 31.0  && absz < 331.5 )
      area = VertexRegion::insidePixelBarrel0;
    else if ( rad < 38.4  && absz < 331.5 )
      area = VertexRegion::aroundPixelBarrel0;
    else if ( rad < 47.7  && absz < 400.5 )
      area = VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1;
    else if ( rad < 54.4  && absz < 400.5 )
      area = VertexRegion::aroundPixelBarrel1;
    else if ( rad < 85.5  && absz < 400.5 )
      area = VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2;
    else if ( rad < 92.2  && absz < 400.5 )
      area = VertexRegion::aroundPixelBarrel2;
    else if ( rad < 119.3 && absz < 400.5 )
      area = VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3;
    else if ( rad < 126.1 && absz < 400.5 )
      area = VertexRegion::aroundPixelBarrel3;
    else if ( rad < 290   && absz < 749.0 )
      area = VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0;
    else if ( rad < 315   && absz < 749.0 )
      area = VertexRegion::aroundSctBarrel0;
    else if ( rad < 360   && absz < 749.0 )
      area = VertexRegion::outsideSctBarrel0_and_insideSctBarrel1;
    else if ( rad < 390   && absz < 749.0 )
      area = VertexRegion::aroundSctBarrel1;
    else
      area = VertexRegion::elsewhere;

    return area;
  }


  //_____________________________________________________________________________________________

  /* inner / outer hit pattern condition */

  template<>
  bool patternMatchInOut< VertexRegion::insideBeamPipe >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    return true;
  }

  template<>
  bool patternMatchInOut< VertexRegion::insidePixelBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    return true;
  }

  template<>
  bool patternMatchInOut< VertexRegion::aroundPixelBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }

  template<>
  bool patternMatchInOut< VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }

  template<>
  bool patternMatchInOut< VertexRegion::aroundPixelBarrel1 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }

  template<>
  bool patternMatchInOut< VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::aroundPixelBarrel2 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::aroundPixelBarrel3 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel0   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1   ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel0   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1   ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::aroundSctBarrel0 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2   ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::sctBarrel0   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2   ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::aroundSctBarrel1 >
  ( const uint32_t& pattern ) {
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    if (  ( pattern & ( 1 << VsiTool::sctBarrel0   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2   ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel3   ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchInOut< VertexRegion::elsewhere >
  ( const uint32_t& /*pattern*/ ) {
    return true;
  }


  /* loose outer-only hit pattern condition */

  template<>
  bool patternMatchOuterOnly< VertexRegion::insideBeamPipe >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::insidePixelBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::aroundPixelBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1<<VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1<<VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::aroundPixelBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::aroundPixelBarrel2 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::aroundPixelBarrel3 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::aroundSctBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::aroundSctBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnly< VertexRegion::elsewhere >
  ( const uint32_t& /*pattern*/ ) {
    return true;
  }


  /* tight outer-only hit pattern condition */

  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::insideBeamPipe >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::insidePixelBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel0 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::aroundPixelBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::aroundPixelBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::pixelBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::aroundPixelBarrel2 >
  ( const uint32_t& pattern ) {
    if ( !hasHit( pattern, VsiTool::pixelBarrel3 ) )               return false;   
    auto outer_sctCond1 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctBarrel1 ) );
    auto outer_sctCond2 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap0 ) );
    auto outer_sctCond3 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap1 ) &&
			    hasHit( pattern, VsiTool::sctEndCap2 ) ); 
    if( ! ( outer_sctCond1 || outer_sctCond2 || outer_sctCond3 ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1<< VsiTool::pixelBarrel3 ) ) )            return false;
    auto outer_sctCond1 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctBarrel1 ) );
    auto outer_sctCond2 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap0 ) );
    auto outer_sctCond3 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap1 ) &&
			    hasHit( pattern, VsiTool::sctEndCap2 ) );  
    if( ! ( outer_sctCond1 || outer_sctCond2 || outer_sctCond3 ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::aroundPixelBarrel3 >
  ( const uint32_t& pattern ) { 
    auto outer_sctCond1 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctBarrel1 ) &&
			    hasHit( pattern, VsiTool::sctBarrel2 ) );
    auto outer_sctCond2 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctBarrel1 ) &&
			    hasHit( pattern, VsiTool::sctEndCap0 ) );
    auto outer_sctCond3 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap1 ) );
    auto outer_sctCond4 = ( hasHit( pattern, VsiTool::sctEndCap0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap1 ) &&
			    hasHit( pattern, VsiTool::sctEndCap2 ) );
    if ( !( outer_sctCond1 || outer_sctCond2 ||
	    outer_sctCond3 || outer_sctCond4 ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 >
  ( const uint32_t& pattern ) {
    auto outer_sctCond1 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctBarrel1 ) &&
			    hasHit( pattern, VsiTool::sctBarrel2 ) );
    auto outer_sctCond2 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctBarrel1 ) &&
			    hasHit( pattern, VsiTool::sctEndCap0 ) );
    auto outer_sctCond3 = ( hasHit( pattern, VsiTool::sctBarrel0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap1 ) );
    auto outer_sctCond4 = ( hasHit( pattern, VsiTool::sctEndCap0 ) &&
			    hasHit( pattern, VsiTool::sctEndCap1 ) &&
			    hasHit( pattern, VsiTool::sctEndCap2 ) );
    if ( !( outer_sctCond1 || outer_sctCond2 ||
	    outer_sctCond3 || outer_sctCond4 ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::aroundSctBarrel0 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel1 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::aroundSctBarrel1 >
  ( const uint32_t& pattern ) {
    if ( !( pattern & ( 1 << VsiTool::sctBarrel2 ) ) ) return false;
    if ( !( pattern & ( 1 << VsiTool::sctBarrel3 ) ) ) return false;
    return true;
  }
  
  template<>
  bool patternMatchOuterOnlyTight< VertexRegion::elsewhere >
  ( const uint32_t& /*pattern*/ ) {
    return true;
  }


  //_____________________________________________________________________________________________

#define CASEPATTERN_INOUT( REGION ) case REGION: \
  flag = patternMatchInOut<REGION>( pattern ); break;

#define CASEPATTERN_INOUT_TIGHT( REGION ) case REGION: \
  flag = patternMatchInOut<REGION>( pattern ); break;

#define CASEPATTERN_OUTERONLY( REGION ) case REGION: \
  flag = patternMatchOuterOnly<REGION>( pattern ); break;

#define CASEPATTERN_OUTERONLY_TIGHT( REGION ) case REGION: \
  flag = patternMatchOuterOnlyTight<REGION>( pattern ); break;


  bool passLoosePattern ( const uint32_t& pattern, const enum VertexRegion& region,
			  const TVector3& trackRelDir )
  {
    bool flag = false;
    if ( fabs( trackRelDir.Phi() ) < TMath::PiOver2() ) {
      switch( region ) {
	CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::insideBeamPipe );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::insidePixelBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel2 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel3 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundSctBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundSctBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::elsewhere );
      default:
	flag = true;
      }
    }
    else {
      switch( region ) {
        CASEPATTERN_OUTERONLY( VertexRegion::insideBeamPipe );
        CASEPATTERN_OUTERONLY( VertexRegion::insidePixelBarrel0 );
        CASEPATTERN_OUTERONLY( VertexRegion::aroundPixelBarrel0 );
        CASEPATTERN_OUTERONLY( VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 );
        CASEPATTERN_OUTERONLY( VertexRegion::aroundPixelBarrel1 );
        CASEPATTERN_OUTERONLY( VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 );
        CASEPATTERN_OUTERONLY( VertexRegion::aroundPixelBarrel2 );
        CASEPATTERN_OUTERONLY( VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 );
        CASEPATTERN_OUTERONLY( VertexRegion::aroundPixelBarrel3 );
        CASEPATTERN_OUTERONLY( VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 );
        CASEPATTERN_OUTERONLY( VertexRegion::aroundSctBarrel0 );
        CASEPATTERN_OUTERONLY( VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 );
        CASEPATTERN_OUTERONLY( VertexRegion::aroundSctBarrel1 );
        CASEPATTERN_OUTERONLY( VertexRegion::elsewhere );
      default:
        flag = true;
      }
    }
    return flag;
  }


  bool passMediumPattern ( const uint32_t& pattern, const enum VertexRegion& region,
			   const TVector3& trackRelDir )
  {
    bool flag = false;
    if ( fabs( trackRelDir.Phi() ) < TMath::PiOver2() ) {
      switch( region ) {
        CASEPATTERN_INOUT( VertexRegion::insideBeamPipe );
        CASEPATTERN_INOUT( VertexRegion::insidePixelBarrel0 );
        CASEPATTERN_INOUT( VertexRegion::aroundPixelBarrel0 );
        CASEPATTERN_INOUT( VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 );
        CASEPATTERN_INOUT( VertexRegion::aroundPixelBarrel1 );
        CASEPATTERN_INOUT( VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 );
        CASEPATTERN_INOUT( VertexRegion::aroundPixelBarrel2 );
        CASEPATTERN_INOUT( VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 );
        CASEPATTERN_INOUT( VertexRegion::aroundPixelBarrel3 );
        CASEPATTERN_INOUT( VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 );
        CASEPATTERN_INOUT( VertexRegion::aroundSctBarrel0 );
        CASEPATTERN_INOUT( VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 );
        CASEPATTERN_INOUT( VertexRegion::aroundSctBarrel1 );
        CASEPATTERN_INOUT( VertexRegion::elsewhere );
      default:
        flag = true;
      }
    }
    else {
      switch( region ) {
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::insideBeamPipe );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::insidePixelBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel2 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundPixelBarrel3 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundSctBarrel0 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::aroundSctBarrel1 );
        CASEPATTERN_OUTERONLY_TIGHT( VertexRegion::elsewhere );
      default:
        flag = true;
      }
    }
    return flag;
  }


  bool passTightPattern ( const uint32_t& pattern, const enum VertexRegion& region,
			  const TVector3& /* trackRelDir */ )
  {
    bool flag = false;
    switch( region ) {
      CASEPATTERN_INOUT_TIGHT( VertexRegion::insideBeamPipe );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::insidePixelBarrel0 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::aroundPixelBarrel0 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::outsidePixelBarrel0_and_insidePixelBarrel1 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::aroundPixelBarrel1 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::outsidePixelBarrel1_and_insidePixelBarrel2 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::aroundPixelBarrel2 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::outsidePixelBarrel2_and_insidePixelBarrel3 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::aroundPixelBarrel3 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::outsidePixelBarrel3_and_insideSctBarrel0 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::aroundSctBarrel0 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::outsideSctBarrel0_and_insideSctBarrel1 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::aroundSctBarrel1 );
      CASEPATTERN_INOUT_TIGHT( VertexRegion::elsewhere );
    default:
      flag = true;
    }
    return flag;
  }


  uint8_t nPixelBarrelLayers ( const uint32_t& pattern ) {
    uint8_t n = 0;
    n += ( ( pattern & ( 1 << DetectorType::pixelBarrel0 ) ) >> DetectorType::pixelBarrel0 );
    n += ( ( pattern & ( 1 << DetectorType::pixelBarrel1 ) ) >> DetectorType::pixelBarrel1 );
    n += ( ( pattern & ( 1 << DetectorType::pixelBarrel2 ) ) >> DetectorType::pixelBarrel2 );
    n += ( ( pattern & ( 1 << DetectorType::pixelBarrel3 ) ) >> DetectorType::pixelBarrel3 );
    return n;
  }

  uint8_t nPixelEndCapLayers ( const uint32_t& pattern ) {
    uint8_t n = 0;
    n += ( ( pattern & ( 1 << DetectorType::pixelEndCap0 ) ) >> DetectorType::pixelEndCap0 );
    n += ( ( pattern & ( 1 << DetectorType::pixelEndCap1 ) ) >> DetectorType::pixelEndCap1 );
    n += ( ( pattern & ( 1 << DetectorType::pixelEndCap2 ) ) >> DetectorType::pixelEndCap2 );
    return n;
  }

  uint8_t nSctBarrelLayers ( const uint32_t& pattern ) {
    uint8_t n = 0;
    n += ( ( pattern & ( 1 << DetectorType::sctBarrel0 ) ) >> DetectorType::sctBarrel0 );
    n += ( ( pattern & ( 1 << DetectorType::sctBarrel1 ) ) >> DetectorType::sctBarrel1 );
    n += ( ( pattern & ( 1 << DetectorType::sctBarrel2 ) ) >> DetectorType::sctBarrel2 );
    n += ( ( pattern & ( 1 << DetectorType::sctBarrel3 ) ) >> DetectorType::sctBarrel3 );
    return n;
  }

  uint8_t nSctEndCapLayers ( const uint32_t& pattern ) {
    uint8_t n = 0;
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap0 ) ) >> DetectorType::sctEndCap0 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap1 ) ) >> DetectorType::sctEndCap1 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap2 ) ) >> DetectorType::sctEndCap2 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap3 ) ) >> DetectorType::sctEndCap3 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap4 ) ) >> DetectorType::sctEndCap4 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap5 ) ) >> DetectorType::sctEndCap5 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap6 ) ) >> DetectorType::sctEndCap6 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap7 ) ) >> DetectorType::sctEndCap7 );
    n += ( ( pattern & ( 1 << DetectorType::sctEndCap8 ) ) >> DetectorType::sctEndCap8 );
    return n;
  }


  #undef CASEPATTERN_INOUT
  #undef CASEPATTERN_INOUT_TIGHT
  #undef CASEPATTERN_OUTERONLY
  #undef CASEPATTERN_OUTERONLY_TIGHT
  
} // VsiTool
