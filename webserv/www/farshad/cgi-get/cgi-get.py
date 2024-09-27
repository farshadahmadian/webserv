#!/usr/bin/env python
import os
from datetime import datetime
try:
    # Python 2
    import urlparse
except ImportError:
    # Python 3
    import urllib.parse as urlparse

# while True:
# 	pass



string_value = os.environ.get('number', '')
number = int(string_value)
number *= 2

current_time = datetime.now().strftime('%H:%M:%S')

html = '''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Document</title>
    <style>
				body {{
					padding: 40px 80px;
				}}

        h1 {{
          display: flex;
          justify-content: center;
          align-items: center;
					margin-top: 100px;
        }}
    </style>
</head>
<body>
<span id="time">Time: {time}</span>
<h1>Result is {number} </h1>
</body>
<script>
    function updateTime() {{
        var now = new Date();
        document.getElementById('time').textContent = now.toLocaleTimeString();
    }}
    setInterval(updateTime, 1000);
    updateTime();
</script>

</html>'''.format(number=number, time=current_time)

print(html)
