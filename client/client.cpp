#include "client.h"

#include "node.h"

static Node* s_scheduler = 0;
Node* scheduler()
{
    return s_scheduler;
}

Client::Client()
{
}
