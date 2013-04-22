#include "Event.h"

namespace ma 
{

    EventListener::EventListener()
    {
        prev = next = NULL;
    }

    EventListener::~EventListener()
    {
        prev = next = NULL;
    }






    Event::Event()
		: head(NULL)
    {
    }

    Event::~Event()
    {
        assert (head == NULL);
    }

    void Event::Call(void * param0, void * param1)
    {
        EventListener * el = head;

        while (el)
        {
            el->OnCall(this, param0, param1);

            el = el->next;
        }
    }

    void Event::operator += (EventListener * p)
    {
        assert (p->prev == NULL && p->next == NULL);

        if (head)
            head->prev = p;

        p->next = head;
        head = p;
    }

    void Event::operator -= (EventListener * p)
    {
        if (head == p)
        {
            head = p->next;
        }
        else
        {
            if (p->prev)
                p->prev->next = p->next;

            if (p->next)
                p->next->prev = p->prev;
        }

        p->prev = p->next = NULL;
    }
}