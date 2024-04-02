#include <string>
#include <fstream>

struct Block
{
    int Y;
    int X;
    bool operator==(const Block& other_block)
    const
    {
        return X == other_block.X && Y == other_block.Y;
    }

    bool operator!=(const Block& other_block)
    const
    {
        return X != other_block.X || Y != other_block.Y;
    }
};

struct queue_path
{
    Block path[1000];
    int head;
    int tail;
    queue_path()
    {
        head = -1;
        tail = -1;
    }

    void enqueue(Block);
    Block dequeue();
    Block peek();
    bool isEmpty();
    bool isFull();
};

bool queue_path::isEmpty()
{
    if (tail == -1) return true;
    else return false;
}

bool queue_path::isFull()
{
    if (head == 999) return true;
    return false;
}

void queue_path::enqueue(Block b_first)
{
    if (head == -1)
    {
        head++;
        tail++;
        path[head] = b_first;
    }
    else
    {
        head++;
        path[head] = b_first;
    }
}

Block queue_path::dequeue()
{
    Block err;
    err.X = -1; err.Y = -1;
    if (isEmpty()) return err;
    else
    {
        if (tail == head)
        {
            tail = -1;
            return path[head--];
        }
        return path[tail++];
    }
}

Block queue_path::peek()
{
    Block err;
    err.X = -1; err.Y = -1;
    if (isEmpty()) return err;
    else
    {
        return path[tail];
    }
}
