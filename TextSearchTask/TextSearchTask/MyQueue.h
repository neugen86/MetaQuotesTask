#pragma once

#include <utility>

template <typename T>
class MyQueue
{
    struct Item
    {
        T data;
        Item* next = nullptr;

        Item(T data, Item** prev)
            : data(data)
        {
            if (prev && *prev)
                (*prev)->next = this;
        }
    };

    Item* m_head = nullptr;
    Item* m_tail = nullptr;
	Item* m_current = nullptr;

public:
    MyQueue() = default;

    MyQueue(const MyQueue& other)
        : m_head(copy(other.m_head, &m_tail))
		, m_current(m_head)
    {
    }

    ~MyQueue()
    {
        while (m_head)
        {
            delete popHeadItem();
        }
    }

    MyQueue& operator =(const MyQueue& other)
    {
        if (this != &other)
        {
            MyQueue tmp(other);
            swap(*this, tmp);
        }

        return *this;
    }

    bool empty() const { return !m_head; }

    T pop()
    {
        Item* item = popHeadItem();

		T result = item->data;
		delete item;

        return result;
    }

    void push(T data)
    {
        if (m_head)
        {
            m_tail = new Item(data, &m_tail);
        }
        else
        {
            m_head = new Item(data, nullptr);
            m_current = m_tail = m_head;
        }
    }

	bool getNext(T* value)
	{
		if (!value || !m_current)
			return false;

		*value = m_current->data;
		m_current = m_current->next;

		return true;
	}

	void reset() { m_current = m_head; }

private:
    Item* popHeadItem()
    {
        Item* item = m_head;
        m_head = m_head->next;

		return item;
    }

    Item* copy(Item* value, Item** tail)
    {
        Item* item = new Item(value->data, nullptr);

        if (value->next)
        {
            item->next = copy(value->next, tail);
        }
        else
        {
            *tail = item;
        }

        return item;
    }

    void swap(MyQueue& lhs, MyQueue& rhs) const
    {
        using std::swap;

        swap(lhs.m_head, rhs.m_head);
        swap(lhs.m_tail, rhs.m_tail);
    }
};
