
# This is necessary for rulebook parsing
# part copied from https://gitlab.cern.ch/atlas-trigger-menu/TrigMenuRulebook/blob/acceeb02ea9b982efb1aed08d77c8f6e0d8e1ce6/python/UserFunctions.py

# changed "allowDuplicates = kwargs.pop("allowDuplicates",False)" to "allowDuplicates = kwargs.pop("allowDuplicates",True)"
# comment "print "Will remove primaries for lumi below",reflumi""
# comment "print "Removed %d intermediate primaries" % len(todel)"


from RuleClasses import TriggerRule


def disableLowerLumiPrimaries(rules, reflumi):
	todel = set()
	# print "Will remove primaries for lumi below",reflumi
	for item, ruledict in rules.iteritems():
		if not item.startswith("HLT"): continue
                if "_delayed" in item: continue

		isPrimary = False
		isSupport = False
                isEndOfFill = False
		for lumi, rule in ruledict.iteritems():
			if "comment" in rule and "Support" in rule["comment"]: isSupport = True
                        if "comment" in rule and "EndOfFill" in rule["comment"]:
                          isEndOfFill = True
                          #print "Saving " + item
			if "PS" in rule and rule["PS"] == 1 and not isSupport and lumi>=1 and not isEndOfFill : 
				isPrimary = True 
				primLumi = lumi
			if (not "PS" in rule or rule["PS"] != 1) and lumi<=reflumi and isPrimary and not (lumi<=primLumi):
				todel.add((item,primLumi))
				break
	for item, lumi in todel:
		# Due to the construction xxx.update(dict(map(None,HLT_list,len(HLT_list)*[ { ...
		# deleting one item might automatically delete the identical ones. That's fine but need to check again for key
		if primLumi in rules[item]: del rules[item][primLumi]
	# print "Removed %d intermediate primaries" % len(todel)
	return rules



import collections
class RulebookDict(collections.MutableMapping):
# class RulebookDict():
		"""A dictionary that keeps track of the keys that have been overwritten"""

		def __init__(self, tag="", subtag="",*args, **kwargs):
				self.tag = tag
				if subtag: self.tag += " "+subtag
				self.store = dict()
				self.errors = []
				self.update(dict(*args, **kwargs))	# use the free update to set keys

		def __getitem__(self, key):
				return self.store[key]

		def __setitem__(self, key, value,othertag="",allowDuplicates=False):
				if key in self.store:
					if allowDuplicates:
						pass
						#print 'INFO     : Known duplicated rule',self.errorString(othertag,key)
					else:
						print 'FATAL    : Duplicated rule',self.errorString(othertag,key)
				self.store[key] = value

		def __delitem__(self, key):
				del self.store[key]

		def __iter__(self):
				return iter(self.store)

		def __len__(self):
				return len(self.store)

		#needs same interface as dict, strips out the keyword allowDuplicates
		def update(self, *args, **kwargs): #allowDuplicates = False
				allowDuplicates = kwargs.pop("allowDuplicates",True)
				othertag = ""
				if args and isinstance(args[0],RulebookDict): othertag = args[0].tag
				for k, v in dict(*args, **kwargs).iteritems():
						self.__setitem__(k,v,othertag,allowDuplicates)

		def errorString(self,othertag,k):
				if othertag: thestr = " between %s and %s"%(self.tag,othertag)
				else: 			 thestr = " in %s"%self.tag
				thestr += ": %s"%k
				return thestr

		def has_key(self,key):
				return key in self
