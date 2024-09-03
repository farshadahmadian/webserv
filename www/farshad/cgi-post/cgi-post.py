import os
from datetime import datetime

# while True:
# 	pass

name = os.getenv('NAME', 'Guest') 

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
<h1>Welcome {name}</h1>
</body>
<script>
    function updateTime() {{
        var now = new Date();
        document.getElementById('time').textContent = now.toLocaleTimeString();
    }}
    setInterval(updateTime, 1000);
    updateTime();
</script>

</html>'''.format(name=name, time=current_time)

print(html)
