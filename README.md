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
│---.gitignore
│---embedded.ino
│---LICENSE
│---README.md
│
├───scripts
└───src
    │---car_seat.h
    │---key_fob.h
    │---parameters.h
    │
    └───tests
        │---test_1.h
```


Notable files:
*  `embedded.ino` - This file is the main ino for building any software, either for the car seat, keyfob, or any test.
* `scripts` will contain the scripts for testing and other utilities.
* `src` will contain development code. Everything should be in a folder except the main files: `car_seat.h`, `key_fob.h`, and `parameters.h`
* `parameters.h` will contain the high level constants and defines necessary in other code.
* `car_seat.h, key_fob.h, test_1.h` - Each of these files are responsible for implementing _setup() and _loop(), targeted at their respective item.
* `test/` - This folder holds all of the automated tests. To create other tests, just add a new file in the `tests/` directory called `<test-name>.h`. This will necessitate adding the appropriate #define's in embedded.ino. Just copy the current structure. Note: to work with the automated testing, the macro definition must be the ALL UPPERCASE version of the `<test-name>.h`. After a tests is ran, these will automaticially populate serial output into the `logs` folder.

Note: because of this setup, arduino will force embedded.ino to be placed in a folder named embedded.