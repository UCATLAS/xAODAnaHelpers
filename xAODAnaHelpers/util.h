
// from TMVA....I think...?
std::vector<TString> SplitString(TString& orig, const char separator)
{
  // 'splitV' with the primitive strings
  std::vector<TString> splitV;
  TString splitOpt(orig);
  splitOpt.ReplaceAll("\n"," ");
  splitOpt = splitOpt.Strip(TString::kBoth,separator);

  while (splitOpt.Length()>0) {
    if ( !splitOpt.Contains(separator) ) {
      splitOpt.ReplaceAll(" ",""); // clear empty spaces
      splitV.push_back(splitOpt);
      break;
    }
    else {
      TString toSave = splitOpt(0,splitOpt.First(separator));
      splitV.push_back(toSave);
      splitOpt = splitOpt(splitOpt.First(separator),splitOpt.Length());
    }
    splitOpt = splitOpt.Strip(TString::kLeading,separator);
  }

  return splitV;
} // SplitString

