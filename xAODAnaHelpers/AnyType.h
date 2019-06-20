#ifndef xAODAnaHelpers_AnyType_H
#define xAODAnaHelpers_AnyType_H

/* General type erasure
   Author: Hide Oide ( Hideyuki.Oide@cern.ch )
   Created: 2018-MAR-11
   --> taken from SUSY FactoryTools / SecVtxPUBNote */

#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <type_traits>
#include <typeinfo>
#include <limits>

#include <TH1.h>

// meta programming: judge if type is vector or not
template < typename T >
struct is_vector { static constexpr bool value = false; };

template < typename T, class A >
struct is_vector< std::vector<T, A> > { static constexpr bool value = 1; };

// check validity of meta function
static_assert ( is_vector<int>                :: value == false, "Ill is_vector impl" );
static_assert ( is_vector< std::vector<int> > :: value == true,  "Ill is_vector impl" );


#include <string>
#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

inline std::string demangle ( const char* name )
{
  int status = -4; // some arbitrary value to elminate compiler warning
  
  // enable c++11 by passing flag -std=c++11 to g++
  std::unique_ptr< char, void(*)(void*) > res {
    abi::__cxa_demangle( name, NULL, NULL, &status ),
      std::free
      };
  
  return ( status == 0 ) ? res.get() : name;
}

# else

// does nothing if not g++
inline std::string demangle ( const char* name ) { return name; }

#endif

template < class T >
inline std::string print_type ( const T& t ) { return demangle( typeid(t).name() ); }


// base class
class VarBase {
 public:
  virtual ~VarBase () {}
  virtual const std::type_info& type  () const = 0;
  virtual void                  print () const = 0;
  virtual void                  clear () = 0;
  virtual VarBase*              clone () = 0;
};


// concrete type wrapper class
template < typename T, typename U = void > class VarHolder;

/* partial specialization for non-class type implementation
   --> requires following functions to work:
   --> - std::ostream operator << ( std::ostream&, TYPE ); */
template < typename T >
class VarHolder < T, typename std::enable_if< !std::is_class<T>::value > :: type > :
  public VarBase {
  public:
  VarHolder ( const T& val ) : m_var( val ) {}
    static constexpr T invalid_value = std::numeric_limits<T>::quiet_NaN();
    ~VarHolder () {}
    const std::type_info& type () const { return typeid(T); }
    T* get () { return &m_var; }
    T& operator* () { return m_var; }

    void print () const {
      std::cout << "value = " << m_var << " [ " << print_type( m_var ) << " ], ";
      std::cout << "address = " << &m_var << ", size = " << sizeof( m_var ) << std::endl;
    }

    void clear () { m_var = invalid_value; }
    VarBase* clone () { return new VarHolder<T>( m_var ); }

  private:
    T m_var;
  };

/* partial specialization for vector types
   --> requires following functions to work:
   --> - has a clear() function */
template < typename T >
class VarHolder < T, typename std::enable_if< is_vector<T>::value > :: type > :
  public VarBase {
  public:
  VarHolder ( const T& val ) : m_var( val ) {}
    ~VarHolder () {}
    const std::type_info& type () const { return typeid(T); }
    T* get () { return &m_var; }
    T& operator* () { return m_var; }

    void print () const {
      std::cout << "vector size = " << m_var.size() << ", ";
      std::cout << "type name = [ " << print_type( m_var ) << " ], ";
      std::cout << "address = " << &m_var << ", size = " << sizeof( m_var ) << std::endl;
    }

    void clear () { m_var.clear(); }
    VarBase* clone () { return new VarHolder<T>( m_var ); }

  private:
    T m_var;
  };

/* partial specialization for non-vector class types
   --> requires following functions to work:
   --> - has a clear() function
   --> - has std::ostream operator << (std::ostream&, TYPE); */
template < typename T >
class VarHolder < T, typename std::enable_if< std::is_class<T>::value &&
  !is_vector<T>::value >::type > : public VarBase {
  public:
  VarHolder ( const T& val ) : m_var( val ) {}
    ~VarHolder () {}
    const std::type_info& type () const { return typeid(T); }
    T* get () { return &m_var; }
    T& operator* () { return m_var; }

    void print () const {
      std::cout << m_var << ", type name = [ " << print_type( m_var ) << " ], ";
      std::cout << "address = " << &m_var << ", size = " << sizeof( m_var ) << std::endl;
    }

    void clear () { m_var.clear(); }
    VarBase* clone () { return new VarHolder<T>( m_var ); }

  private:
    T m_var;
  };

/* partial specialization for TH1-derived classes
   --> requires following functions to work:
   --> - inherits TObject
   --> - has GetName() function
   --> - has Clear() function */
template < typename T >
class VarHolder < T, typename std::enable_if< std::is_class<T>::value &&
  !is_vector<T>::value && std::is_base_of<TH1,T>::value >::type > : public VarBase {
  public:
  VarHolder ( const T& val ) : m_var( val ) {}
    ~VarHolder () {}
    const std::type_info& type () const { return typeid(T); }
    T* get () { return &m_var; }
    T& operator* () { return m_var; }

    void print () const {
      std::cout << m_var.GetName() << ", type name = [ " << typeid(std::vector<T>).name();
      std::cout << " ], address = " << &m_var << ", size = " << sizeof( m_var ) << std::endl;
    }

    void clear () { m_var.Clear(); }
    VarBase* clone () { return new VarHolder<T>( m_var ); }

  private:
    T m_var;
  };



class anytype {
 private:
  std::unique_ptr< VarBase > m_var;

 public:
  
  // default constructor
 anytype () : m_var( nullptr ) {}

  // copy constructor
 anytype ( const anytype& a ) : m_var( a.get().clone() ) {}

  // copy constructor
 anytype ( anytype& a ) : m_var( a.get().clone() ) {}

  // move constructor
  anytype ( anytype&& a ) { m_var = std::move( a.m_var ); }

  template < typename T > anytype( T val  ) : m_var( new VarHolder<T>( val ) ) {}
  template < typename T > anytype( T& val ) : m_var( new VarHolder<T>( val ) ) {}
  template < typename T > anytype& operator = ( const T& val ) {
    if ( !m_var ) m_var.reset( new VarHolder<T>( val ) );
    else {
      //std::cout << __PRETTY_FUNCTION__ << ": substituting" << print_type( val ) << std::endl;
      VarHolder<T>& vh = dynamic_cast< VarHolder<T>& >( *m_var );
      T& ref = *vh;
      ref = val;
    }
    return *this;
  }

  ~anytype () {}
  const std::type_info& type () const { return m_var->type(); }
  void print () const { m_var->print(); }
  void clear () { m_var->clear(); }
  VarBase& get () const { return *m_var; }
  template < typename T > void set( T&& val )
    { *( dynamic_cast< VarHolder<T>& >( *m_var ).get() ) = val; }
  template < typename T > T& getVar()
    { return *( dynamic_cast< VarHolder<T>& >( this->get() ) ); }
  template < typename T > T getVar() const
    { return *( dynamic_cast< VarHolder<T>& >( this->get() ) ); }
 
};

#endif // xAODAnaHelpers_AnyType_H
