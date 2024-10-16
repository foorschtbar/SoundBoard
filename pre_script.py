import os
import re
Import("env")

print("External script: Updating WebUI...")

# Read content
html = open('./html/index.html', 'r')
o = html.read()
indexhtml = o
html.close()

# HTML content to C++ string
content = "// WARNING! Do not modify this file, it is auto-generated from index.html at build!\n"
content += "const char index_html[] PROGMEM = R\"=====(" + indexhtml + ")=====\";"

# Write the content of the file
webuih = open('./include/index.h', 'w')
webuih.write(content)
webuih.close()
print("External script: Updating WebUI successful!")