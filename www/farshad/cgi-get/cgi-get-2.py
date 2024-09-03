""" import os

# Initialize an empty string to store HTML content
html_content = ""

# Iterate over the environment variables
for key, value in os.environ.items():
    # Append each environment variable in a <p> tag
    html_content += "<p>{}: {}</p>\n".format(key, value)

# Print the HTML content
print(html_content) """

""" import os

# Initialize an empty string to store HTML content
html_content = ""

# Convert environment variables to a list of tuples for easy indexing
env_items = list(os.environ.items())

# Iterate over the environment variables, skipping the first and last
for key, value in env_items[1:-1]:
	# Append each environment variable in a <p> tag
	html_content += "<p>{}: {}</p>\n".format(key, value)

# Print the HTML content
print(html_content)
 """

""" import os

# Initialize an empty string to store HTML content
html = '''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Document</title>
    <style>
				body {
					padding: 40px 80px;
				}

        h1 {
					margin-top: 100px;
        }
    </style>
</head>
<body>
<span id="time">Time: {time}</span>
<h1>Here is your data </h1>
</body>
<script>
    function updateTime() {{
        var now = new Date();
        document.getElementById('time').textContent = now.toLocaleTimeString();
    }}
    setInterval(updateTime, 1000);
    updateTime();
</script>

</html>'''

# Convert environment variables to a list of tuples for easy indexing
env_items = list(os.environ.items())

# Reverse the list of environment variables and skip the first and last ones
reversed_env_items = env_items[1:-1][::-1]

# Iterate over the reversed environment variables
for key, value in reversed_env_items:
	# Append each environment variable in a <p> tag
	html += "<p>{}: {}</p>\n".format(key, value)

# Print the HTML content
print(html) """





import os

html = '''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Document</title>
    <style>
				body {
					padding: 40px 80px;
				}

        h1 {
					margin-top: 100px;
        }
    </style>
</head>
<body>
<span id="time">Time: {time}</span>
<h1>Here is your data </h1>
</body>
<script>
    function updateTime() {{
        var now = new Date();
        document.getElementById('time').textContent = now.toLocaleTimeString();
    }}
    setInterval(updateTime, 1000);
    updateTime();
</script>

</html>'''

for key, value in os.environ.items():
    if key not in ["VERSIONER_PYTHON_VERSION", "__CF_USER_TEXT_ENCODING"]:
			html += "<p>{}: {}</p>\n".format(key, value)

print(html)
