// SSH Class hold the data to establish the connection to the SSH server. Required by secureShell() and secureCopy().

#ifndef SSH_H
#define SSH_H

class SSH {
private:
    char * _userSSH;
    char * _serverSSH;
public:
    SSH ();
    void   setUserSSH   (char * _newUserSSH);
    void   setServerSSH (char * _newServerSSH);
    char * getUserSSH   ();
    char * getServerSSH ();
    void   printConnectionData();
};


#endif //SSH_H
