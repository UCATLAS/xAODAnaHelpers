
# taken from https://gitlab.cern.ch/atlas-trigger-menu/TrigMenuRulebook/blob/master/python/RuleClasses.py on 2018-03-12
# not otherwise modified


"""A collection of classes for the specification of trigger rules and
the calculation of prescales for running online

In stand-alone mode, this script can be used to upload and download rulebooks:

eg. 
python RuleClasses.py --download="test.py" --active
python RuleClasses.py --upload="test.py"

For more details:
python RuleClasses.py --help 
"""

class TriggerRule:
    """Representation of the rate rules an item should obey
    Valid keys are: tag, rate, maxRate, inputRate, inputMaxRate, PS, fraction, depends_on, PTRate, ESRate, comment, rerun, scaling

    For example: 
       TriggerRule(rate=10) would signify that an item is expected to produce a rate of 10Hz
       TriggerRule(PS=1) would signify that an item is expected to have a prescale of 1
    
    """

    allowed = set(["rate", "maxRate", "minRate", "inputRate", "inputMaxRate",
                   "PS", "PS_slope", "propagate",
                   "fraction", "depends_on", "PTRate","PTValue", 
                   "ESRate", "ESValue", "comment", "rerun", "scaling", "lumi",
                   "end_of_fill", "reoptimize"])
    
    #These type rules will be enforced
    float_keys = ["rate", "maxRate", "inputRate", "inputMaxRate", "PTRate", "ESRate", "PS",  "PS_slope","PTValue","ESValue", "fraction", "end_of_fill", "rerun"]
    int_keys = ["lumi"]
    bool_keys  = ["propagate", "reoptimize"]
    allowed_scales = ["lumi", "bunches"]

    def __str__(self):
        output = '%s(' % self.__class__.__name__
        for (key, value) in self.rules.items():
            if isinstance(value, str):
                value = "'%s'" % value
            output += "%s=%s," % (key, value)
        output += ')'

        return output

    def __repr__(self):
        return self.__str__()

    def __getitem__(self, key):
        return self.rules[key]

    def __setitem__(self, key, value):
        if key not in self.allowed:
            raise KeyError('Using an invalid key: %s' % key)
 
        if key in self.float_keys:
            self.rules[key] = float(value)

        elif key in self.bool_keys:
            self.rules[key] = bool(value)

        elif key in self.int_keys:
            self.rules[key] = int(value)
            
        else:
            self.rules[key] = value
        
        
    def __contains__(self, key):
        return key in self.rules

    def keys(self):
        """Return the rule specifications we have"""
        return self.rules.keys()

    def get(self, key, default):
        if key in self:
            return self.rules[key]
        return default

    def __init__(self, **kwargs):


        #Check that values were pased
        if len(kwargs) == 0:
            raise ValueError("Cannot define a rule with no params: %s" % kwargs)

        #Check allowed tags here
        #if not set(kwargs.keys()) <= self.allowed:
        #    raise KeyError('Using an invalid key: %s' % kwargs)
        
        ## It's easier for users to find the offensive rule if only
        ## the invalid key is printed
        for key in kwargs.keys():
            if key not in self.allowed:
                raise KeyError('Using an invalid key: %s' % key)
        
        #Do collision tests here. e.g. no defining a PS and rate
        collision_terms = [
            ["rate", "inputRate", "PS", "fraction"],
            ["PTRate", "PTValue"],
            ["ESRate", "ESValue"]
            ]
        
        for terms in collision_terms:
            if len([key for key in terms if key in kwargs]) > 1:
                raise KeyError("Cannot define colliding rules: %s" % kwargs)    

        #Check for missing terms
        if 'maxRate' in kwargs and not any(x in kwargs for x in ['rate','PS']):
            raise KeyError('Must define rate or PS with maxRate: %s' % kwargs)
        if 'fraction' in kwargs and len(kwargs.get('depends_on', ',').split(",")) != 1:
            raise KeyError('Must define a single depends_on with fraction: %s' % kwargs)

        if 'depends_on' in kwargs and \
           (('rate' in kwargs and not 'maxRate' in kwargs) or \
            ('inputRate' in kwargs and not 'inputMaxRate' in kwargs)
            ):
            raise KeyError('rate and inputRate with a depends_on must specify a max: %s' % kwargs)
        
        #Store the keywords
        self.rules = kwargs.copy()

        #Some type protections
        for key in self.rules:
            if key in self.float_keys:
                self.rules[key] = float(self.rules[key])

            elif key in self.bool_keys:
                self.rules[key] = bool(self.rules[key])

            elif key in self.int_keys:
                self.rules[key] = int(self.rules[key])

        if "scaling" in self.rules and self.rules["scaling"] not in self.allowed_scales:
            raise ValueError("Scaling must be one of the following: %s" % self.allowed_scales)
  
        if 'ESValue' in self.rules:
            if self.rules['ESValue'] < 0.: 
                raise ValueError('ESValue can not be negative, please fix')

if True:

    baseurl="http://atlas-trigconf.cern.ch"

    import urllib2
    from urllib import urlencode
    import hashlib
    def download(run_type, status):
        req = urllib2.Request(url = baseurl+"/rulebook/rule/download",
                              data = urlencode([('run_type', run_type), ('status', status)]),
                              headers = {'Accept': 'text/plain',
                                         'Content-Type': 'application/x-www-form-urlencoded'})
        try:
            downloaded_text = urllib2.urlopen(req).read()
        except urllib2.HTTPError, error:
            print error.read()
        else:
            
            if '{' == downloaded_text[0]:
                return eval(downloaded_text)
            else:
                print downloaded_text
                return {}


    def upload(run_type, memo, rules):
        import getpass
        user = raw_input('db user name:')
        pwd  = getpass.getpass('db password for:')

        req = urllib2.Request(url = baseurl+"/rulebook/rule/upload",
                              data=urlencode([('run_type', run_type),
                                              ('memo', memo),
                                              ('user', user),
                                              ('pwd', hashlib.sha1(pwd).hexdigest()),
                                              ('rules', str(rules)),]) )
        try:
            print urllib2.urlopen(req).read()
        except urllib2.HTTPError, error:
            print error.read()


    def main(options, args):
        if (options.active or options.pending or options.dropped) and \
               options.download == "":
            print "ERROR: --active, --pending, and --dropped may only be specified in conjunction with --download"
            return
        
        if options.download != "" and options.upload != "":
            print "ERROR: Cannot specify both --upload and --download"
            return
        
        if options.download == "" and options.upload == "":
            print "ERROR: Must specify one of --upload or --download"
            return

        if options.download != "":
            stat = ""
            if options.active:
                stat += "a"
            if options.pending:
                stat += "p"
            if options.dropped:
                stat += "d"

            if stat == "":
                print "ERROR: Must specify at least one of --active, --pending, or --dropped in conjunction with --download"
                return
            
            output = open(options.download, 'w')
            output.write("from RuleClasses import TriggerRule\n")
            output.write("\n")
            output.write("\n")
            output.write("tag = '%s'\n" % options.run_type)
            output.write("\n")
            output.write("\n")
            rules = download(run_type=options.run_type, status=stat)

            # make the output nice for editing
            import pprint
            output.write("rules = {\n%s\n}" %   '\n'.join([ "  %-35s: %s,"% ("'"+trigger+"'", pprint.pformat(rules[trigger])) for trigger in sorted(rules.keys()) ]) )
            output.close()
            
        if options.upload != "":
            if options.memo == "":
                print "ERROR: Must specify a memo when uploading"
                return
            
            if (options.upload[-3:] == ".py"):
                options.upload = options.upload[:-3]
            to_upload = __import__(options.upload)

            confirm=raw_input("Will upload the rules from %s, please confirm [y/n]" % options.upload)
            
            if (confirm == "y"):
                upload(run_type=options.run_type, memo=options.memo, rules=to_upload.rules)
            else:
                print "User responded with '%s', quiting" % confirm
                return
        
if __name__ == "__main__": 
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-d"        , "--download"        , type="str"     , dest="download" , default="" , help="perform a download of the trigger rules to the speficied file")
    parser.add_option("-u"        , "--upload"          , type="str"     , dest="upload"   , default="" , help="perform an upload of the specified trigger rules file")
    parser.add_option("--run_type", type="str", dest="run_type", default="Physics_pp_v2", help="The desired rule's run type field")
    parser.add_option("--active"  , action="store_true" , dest="active"  , help="when downloading select the active rules")
    parser.add_option("--pending" , action="store_true" , dest="pending" , help="when downloading select the pending rules")
    parser.add_option("--dropped" , action="store_true" , dest="dropped" , help="when downloading select the dropped rules")
    parser.add_option("--memo", type="str", dest="memo", default="", help="upload a memo with the trigger rules")

    
    (options, args) = parser.parse_args()
    main(options, args)

    
    
