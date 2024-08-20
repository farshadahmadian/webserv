#!/usr/bin/python3

import os
import subprocess
import time
print("<html><body><center><h1>Hello from Python!</h1></center></body></html>")

print("\n----- THE SCRIPT COMMAND -----\n")

# Call 'ls -la' command
result = subprocess.run(['ls', '-la'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

# Output the result
if result.returncode == 0:
    print(result.stdout)
else:
    print("Error:", result.stderr)

print("\n----- THE ENV VARIABLES -----\n")

# content_type = os.environ.get('SERVER_PROTOCOL')
# if content_type:
# 	print(f'content-type: {content_type}')
# else:
#      print('content-type: is not set')
     

# host_name = os.environ.get('HOST_NAME')

# if host_name:
#     print(f'host-name: {host_name}')
# else:
#     print('host-name: is not set')

# path_info = os.environ.get('PATH_INFO')

# if path_info:
#     print(f'PATH_INFO: {path_info}')
# else:
#     print('path-info: is not set')

# Iterate over all environment variables and print them
for key, value in os.environ.items():
    print(f'{key}: {value}')


# Infinite loop
# while True:
#     print("This is an infinite loop...")
#     time.sleep(1)  # Sleep for 1 second between prints to slow down the loop