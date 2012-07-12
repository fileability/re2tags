import json
import os
from ConfigParser import RawConfigParser
import io

def normalizeRegex(r):
    r = r.replace('IDENTS', r'(?:[a-zA-Z_][a-zA-Z0-9]*(?:\s*,\s*[a-zA-Z_][a-zA-Z0-9]*)*)')
    r = r.replace('IDENT', r'(?:[a-zA-Z_][a-zA-Z0-9]*)')
    
    r = r.replace('{{', '(?P<names>')
    r = r.replace('}}', ')')
    
    r = r.replace(' +', r'[ \t]+')
    r = r.replace(' ?', r'[ \t]?')
    r = r.replace(' *', r'[ \t]*')
    r = r.replace(' ',  r'[ \t]+')
    
    if not r.startswith('^'):
        r = r'^\s*' + r
    return r
    
root = os.path.split(os.path.abspath( __file__ ))[0]
os.chdir(root)

definitions = {}
for p in os.listdir('.'):
    if os.path.splitext(p)[1] != '.ini':
        continue
    
    j = {
        "exts": [],
        "symbols": [],
    }
    with open(p, 'r') as f:
        inicontents = f.read()
        rcp = RawConfigParser()
        rcp.readfp(io.BytesIO(inicontents))
        
        sections = [s for s in rcp.sections() if s != 'language']
        
        j["exts"] = rcp.get("language", "exts").strip().split()
        for section in sections:
            scopes = rcp.get(section, "scope").strip().split(', ') if rcp.has_option(section, "scope") else []
            j["symbols"].append({
                "regex": rcp.get(section, "regex"),
                "scope": scopes,
            })
    
    for symbol in j["symbols"]:
        if "regex" in symbol and symbol["regex"]:
            symbol["regex"] = normalizeRegex(symbol["regex"])
    
    definitions[os.path.splitext(p)[0]] = j

minified = True
with open('compiled.json', 'w') as f:
    if minified:
        f.write(json.dumps(definitions, separators=(',',':')))
    else:
        f.write(json.dumps(definitions, sort_keys=True, indent=4))
