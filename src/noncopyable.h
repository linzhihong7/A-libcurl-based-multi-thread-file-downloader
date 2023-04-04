#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

class noncopyable
{
private:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

#endif