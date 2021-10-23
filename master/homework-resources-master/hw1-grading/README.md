# How To Grade

In order to use this grading artifact, install `python3.7` on your Docker container or virtual machine.
Then, run the following:

```
$ python3.7 -m pip install curricula curricula-shell
```

To then actually grade your assignment, you'll need the following paths:

- `<path_to_hw1>` is the path to your folder containing all of your hw1 submission.
- `<path_to_hw1-grading>` is the path to this folder.

Substitute them into the following command:

```
python3.7 -m curricula_shell grade <path_to_hw1-grading> run <path_to_hw1> --report -d .
```

If you see that cases failed, you can look at the `*.report.json` file that will show up in the directory you ran the command from.
For most of the cases, there will be some kind of message.
