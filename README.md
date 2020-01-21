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
