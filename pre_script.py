import os
import re
import gzip
#Import("env")

print("External script: Compressing WebUI...")

# Paths to the original and compressed files
input_file = 'html/index.html'
output_file = 'html/index.html.gz'

# Open the input HTML file and the output gzip file
with open(input_file, 'rb') as f_in, gzip.open(output_file, 'wb') as f_out:
    # Compress the HTML content
    f_out.writelines(f_in)

print("External script: Updating WebUI successful!")
