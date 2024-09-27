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
