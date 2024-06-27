#quick and dirty CLI for testing and fuzzing
#TODO: add wasmtime support

import base64
import subprocess

def processBattle( warrior_1, warrior_2 ):
    def rebase( b64_str ):
        raw = base64.b64decode(b64_str)
        return base64.b64encode(raw).decode("utf-8")
    warrior_1 = rebase( warrior_1 )
    warrior_2 = rebase( warrior_2 )

    # Command to execute
    command = "./build/vm "+warrior_1+" "+warrior_2

    # Run the command and capture the output
    result = subprocess.run(command, stdout=subprocess.PIPE, shell=True, text=True)

    # Parse the output as an integer
    try:
        output_as_integer = int(result.stdout.strip())
        return output_as_integer
    except ValueError as e:
        return -1


result = processBattle("ACAAAQAk9P8AKBQAAEEAAAEgAgABJAIAAigBAAQoAAB5cOj/dXAEAAAgAAA=", "ACAUAABQvroBIAQAdXD0/w==")
print( "Result=%d"% result )
#./vm ACAAAQAk9P8AKBQAAEEAAAEgAgABJAIAAigBAAQoAAB5cOj/dXAEAAAgAAA= ACAUAABQvroBIAQAdXD0/w==
