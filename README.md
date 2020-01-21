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
│---car_seat.ino
│---key_fob.ino
|---test_<submodule-name>.ino
│---LICENSE
│---README.md
│
├───scripts
├───logs
└───src
    |---parameters.h
```


Notable files:
*  ```car_seat.ino``` - This file is the main ino for the car seat embedded software.
*  ```key_fob.ino``` - This file is the main ino for the key fob embedded software.
*  ```test_<submodule-name>.ino``` - This file is the main test ino for the barebones unit test of each submodule. Examples could be test_power.ino or test_bluetooth.ino. These will automaticially populate serial output into the ```logs``` folder.
* ```scripts``` will contain the scripts for testing and other utilities.
* ```src``` will contain development code. Write your .cpp and .h files here.
* ```parameters.h``` will contain the high level constants and defines necessary in other code. 

