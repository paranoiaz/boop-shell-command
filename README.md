# Boop Shell Command
Create and remove files and modify timestamps with a fast shell command.

**Notes**

Compile boop using ```make``` then run ```make exec``` to get started. Add the absolute path of the directory with the executable in it to your PATH to be able to use the command anywhere. Use ```make clean``` to remove boop.

**Usage**
```
usage: boop [ARGUMENT] FILE ...

        -c
                check if file exist

        -r
                remove if file exist

        -a
                change the file access time

        -m
                change the file modification time

        -t STAMP
                use timestamp in format YYYYMMDDhhmm

        --help
                show this help message and exit

        --version
                show version information and exit
```
