# embedded
Software running on micro-controllers in our Better Baby Buggy!


## Github software repo SOP
1. Create your branch off of master
```
git branch <branch-name>
```
* `<branch-name>` should be all lowercase, and dash(-) separated
2. Work on your code using commit's and stuff, see here: (https://rogerdudler.github.io/git-guide/)
3. Push code as nessasary
4. After completing your feature, create the nessasary unit tests and use them to test your code
5. Once your unit tests work and pass, merge your branch into dev
```
git checkout dev
git merge <branch-name>
git push
```
6. Test the code base buy running all unit tests
7. Once all unit tests pass, merge into master by creating a pull request and merging it
8. :)

## File Structure Layout
The software file structure will be setup as follows.
```
.
│   .gitignore
│   LICENSE
│   notes.txt
│   README.md
│
├───car_seat
│       car_seat.ino
│
├───key_fob
│       key_fob.ino
│
├───libraries
│   ├───example
│   │       example_device_driver.cpp
│   │       example_device_driver.h
│   │       other_example.h
│   │
│   └───parameters
│           parameters.h
│
├───scripts
│
└───tests
    └───example_module_test
            example_module_test.ino
```


Notable files:
*  `car_seat.ino` - This file is the main ino for the car seat.
*  `key_fob.ino` - This file is the main ino for the key fob.
* `libraries` will contain any shared development code. Each library should be in a seperate folder. These can be included in .ino files with ''' #include <example_device_driver.h> '''
* `scripts` will contain the scripts for testing and other utilities.
* `parameters.h` will contain the high level constants and defines necessary in other code.
* `tests/` - This folder holds all of the test .inos. Later on, an automated system will run these test with the hardware in the loop and log serial output into a 'logs' folder. 
