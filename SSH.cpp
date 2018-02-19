
#include "SSH.h"
#include <iostream>

SSH::SSH() {
    _userSSH = (char *) "nuno";
    _serverSSH = (char *) "cfar-athena.me.uvic.ca";
}

void SSH::setUserSSH(char * _newUserSSH) {
    _userSSH = _newUserSSH;
}

void SSH::setServerSSH(char * _newServerSSH) {
    _serverSSH = _newServerSSH;
}

char * SSH::getUserSSH() {
    return _userSSH;
}

char * SSH::getServerSSH() {
    return _serverSSH;
}

void SSH::printConnectionData() {
    std::cout << "\n:::::::::::: SSH CONNECTION" << std::endl;
    std::cout << "       USER: " << _userSSH << std::endl;
    std::cout << "     SERVER: " << _serverSSH << std::endl;
}