# TaskTimer
Command line tool for tracking time spent on tasks.

Made to bring some much needed order to my life when I was a helpdesk technician 

### How to use
In the absence of a makefile it is quite easy to compile:
```
gcc -o tasktimer.exe main.c task.c parse_XML.c
```
Once compiled run with the name of a file to save to/load from as commandline argument:
```
tasktimer tasks.xml
```
(Note that the so called "XML" in this situation is very unsophisticated.)

Once running you will be greeted with a prompt:
```
?>
```
This prompt can actually be overwritten using an easily discovered buffer overflow!

The first thing to know is you can get out of it at any time by typing
```
?>end
```
But if you actually use the thing, create a new task thus:
```
?>n taskname
```
where "taskname" can be a nice little name/description that will remind you what you were doing.

Now you will be in timer mode! The prompt will change so you know what you're doing:
```
taskname?>
```
Now if you want to pause work on this task, simply hit it with
```
taskname?>x
```
This is good if you need to get up and make some tea, a new call comes in, someone comes over to
disract you, you remember that you were actually meant to be working on something else etc.
This will record the current time and update the total time spent on that task accordingly.

Note that if you attempt to create a new task before exiting the one you are currently working
on, the active task will automatically be paused for you.

Another good command is to print all existing tasks
```
?>p
----------------------
ID: 0
Description: testthing
Time spent: 0 mins 44 secs
----------------------
----------------------
ID: 1
Description: test2
Time spent: 0 mins 4 secs
----------------------
----------------------
ID: 2
Description: test3
Time spent: 0 mins 6 secs
----------------------
```
Currently this is the only way to see a task's "ID". Once you know the ID you can use it to resume
work on an existing task
```
?>s 1
test2?>
```
's' stands for "select" I think... or "start"? I'm not sure anymore.

Use 'b' (backup) at any time to write to the flle. This is a good idea to do occasionally -
power might go out, the Hypervisor gave up while someone was tring to uninstall Avast. Who knows, maybe the program
will crash because I've hardcoded a fixed size for the buffer used to construct the string that gets
printed when you type 'p'.

I think that's about it! use "end" at ay time to exit, everything will be saved to the file provided.
