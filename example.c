//Print with sprintf and write
//0 is stdin, 1 is stdout, 2 is stderr

// cat < discard.me
// displays discard.me to the console

//avoid blocking, pipes take priority over stdin
//need a mechanism to check for this
//select() kernel call?

//fcntl might be interesting, non blocking io

//feel free to use a 4k buffer for reading cus why not

//for select
//fd_zero clears file descriptors from a set
//fd_set adds file desriptor fd to set

//something something last file opened plus 1
//readfds is what we're interested in
//select limited to checking 1024 things
//huskyscript will not need a time value

//use man in the middle attacks on your friends and family

printArgs(char **a){

}
int main(int argc, char **argv, char **envp){
    printArgs(argv);

    //make a new argument array
    //malloc and copy
    printArgs(newArgv);

    //se the slides for pipe code
    pipe(c2p);
    //use a separate pipe for the child's stderr
    
    if(fork()==0){
        //dup 2? dup stderr?
        
        close(c2p[0]);

        //closes stdout
        close(1);

        
        //duplicates the c2p[1] file descriptor
        dup(c2p[1]);

        execve(....);


        exit(-1);
    }


    //figure out what he means by this later
    log1 = open("blah/1", O_CREAT ...)
    close(c2p[1]);

    
    l = read(c2p[0], buf, sizeof(buf));
    while(l > 0){
        write(1,buf,l);
        write(log1,buf,l);
        l = read(c2p[0],buf,sizeof(buf));
    }

    wait(NULL);


    return 0;
}