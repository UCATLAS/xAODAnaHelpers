#!/usr/bin/env python
""" Usage: call with <filename>
"""

import sys
import clang.cindex
from itertools import izip
import json

def infoswitch_parser(node, index=0):
  tokens = []
  for token in node.get_tokens():
    if token.spelling == node.spelling: continue
    if token.kind in [clang.cindex.TokenKind.COMMENT, clang.cindex.TokenKind.KEYWORD]: continue
    if token.spelling in ['(', ')', '=', '{', '}']: continue
    # what's the token?
    spelling = token.spelling
    # chunks of 3 (variable, matcing type, string to match)
    if token.kind == clang.cindex.TokenKind.PUNCTUATION:
      if token.spelling not in ';': continue
      else:
        match_type = None
        if 'has_exact' in tokens:
          match_type = 'has_exact'
        elif 'get_working_point' in tokens:
          match_type = 'get_working_point'
        if match_type:
          match_index = tokens.index(match_type)
          identifier = tokens[match_index-1]
          rest = iter(tokens[match_index:])
          for match_type, match_string in izip(rest,rest):
            yield [identifier, match_type, match_string]
          tokens = []
    else:
      # strip quotes
      if token.kind == clang.cindex.TokenKind.LITERAL: spelling = spelling.replace('"','').replace("'",'')
      tokens.append(spelling)
  yield []

def find_inheritance(node):
  if node.kind != clang.cindex.CursorKind.CLASS_DECL: return
  for c in node.get_children():
    if c.kind != clang.cindex.CursorKind.CXX_BASE_SPECIFIER: continue
    children = list(c.get_children())
    if children[0].get_definition():
      return children[0].get_definition().spelling

def find_infoswitches(infoswitches, node):
  if node.spelling == 'initialize' and node.kind == clang.cindex.CursorKind.CXX_METHOD:
    infoswitches[node.semantic_parent.spelling] = [token for token in infoswitch_parser(node) if token]
  for child in node.get_children():
    find_infoswitches(infoswitches, child)

clang.cindex.Config.set_library_path('/usr/local/opt/llvm/lib')
index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1], args=['-x', 'c++'])
print 'Translation unit:', tu.spelling

infoswitches = {}
find_infoswitches(infoswitches, tu.cursor)

for infoswitch, tokens in infoswitches.iteritems():
  print 'Tokens for {}'.format(infoswitch)
  for var, match_type, match_string in tokens:
    print '  -',var, match_type, match_string

print json.dumps(infoswitches, sort_keys=True, indent=4)
