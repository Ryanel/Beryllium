import os 

# List of directories of files to add newlines to.
# Script must be in the same location as these directories.
directories = ["src","src/lib","src/boot","src/drivers","src/fs","src/includes","src/includes/beryllium","src/includes/beryllium/drivers","src/includes/beryllium/fs","src/includes/boot","src/includes/lib","src/includes/x86","src/lib","src/x86/","src/x86/boot/","src/x86/drivers","src/x86/lib","src/x86/low"]

for dir in directories:
    for file in os.listdir(dir):
        if file.endswith(".h") or file.endswith(".c"):
            path = dir + "/" + file
            handle = open(path, "r")
            slurp = handle.read()
            handle.close()

            if not slurp.endswith("\n"):
                retcode = os.system("chmod +w " + path)
                if retcode != 0:
                    print("chmod returned ",retcode," on ",path)
                else:
                    handle = open(path, "a")
                    print("Writing newline to ",path," ...")
                    handle.write("\n")
                    handle.close()

print("Done")
