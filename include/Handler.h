//
// Created by n on 16/06/25.
//

#ifndef HANDLER_H
#define HANDLER_H

#include "ircserv.hpp"


class Handler {
    Client _client;
public:
    Handler();
    Handler(Client &client);
    Handler(Handler &src);
    ~Handler();
    Handler &operator=(Handler &src);

    int accept();
};

#endif //HANDLER_H
