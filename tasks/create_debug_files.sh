#!/bin/bash
echo 'directory /Users/dmitriikononov/Projects/bmstu_cpp_course' > /Users/dmitriikononov/.gdbinit
echo 'add-auto-load-safe-path /' >> /Users/dmitriikononov/.gdbinit
echo 'set auto-load local-gdbinit on' >> /Users/dmitriikononov/.gdbinit
echo 'settings set target.load-cwd-lldbinit true' > /Users/dmitriikononov/.lldbinit
chmod 644 /Users/dmitriikononov/.gdbinit /Users/dmitriikononov/.lldbinit
