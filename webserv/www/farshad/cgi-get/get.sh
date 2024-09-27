#!/bin/bash

# Initialize an empty string to store HTML content
html_content="<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8' /><meta name='viewport' content='width=device-width, initial-scale=1.0' /><title>Document</title><style>body {padding: 40px 80px;}
        h1 {margin-top: 100px;}</style></head><body><h1>Here is your data </h1></body></html>"

# Iterate over the environment variables
for key in $(env | cut -d'=' -f1); do
    # Check if the key is not PWD, SHLVL, or _
    if [[ "$key" != "PWD" && "$key" != "SHLVL" && "$key" != "_" ]]; then
        value="${!key}"
        # Append each environment variable in a <p> tag
        html_content+="<p>${key}: ${value}</p>\n"
    fi
done

# Print the HTML content
echo -e "$html_content"
