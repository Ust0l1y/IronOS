void kentry() {
    char *vptr = (char*) 0xB8000;
    char *msg = "Hello, I am an Architect!";
    int i = 0;
    while(msg[i] != '\0') {
        vptr[i*2] = msg[i];
        vptr[i*2+1] = 0x07;
        i++;
    }
}