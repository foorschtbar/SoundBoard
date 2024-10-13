import os
import re
Import("env")

print("Updating WebUI")

# Read content
html = open('./html/index.html', 'r')
o = html.read()
content = o
html.close()

# HTML content to C++ string
content = "const char index_html[] PROGMEM = R\"=====(" + content + ")=====\";"

# Write the content of the file
webuih = open('./src/index.h', 'w')
webuih.write(content)
webuih.close()
print("WebUI updated")