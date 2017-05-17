#pragma once

template<typename T>
struct CallbackContainer;

template<typename T>
class RefCounter
{
public:
    RefCounter()
        : callbackContainer(nullptr)
          , index(0)
    {}

    RefCounter(size_t index, CallbackContainer<T> *callbackContainer)
        : index(index)
          , callbackContainer(callbackContainer)
    {
        inc();
    }
    RefCounter(const RefCounter<T> &other)
        : callbackContainer(other.callbackContainer)
    {
        inc();
    }

    RefCounter<T> &operator=(const RefCounter<T> &other)
    {
        dec();
        callbackContainer = other.callbackContainer;
        index = other.index;
        inc();
        return *this;
    }

    ~RefCounter()
    {
        dec();
    }

private:
    void inc();
    void dec();
    CallbackContainer<T> *callbackContainer;
    size_t index;
};

template<typename T>
class Callback
{
public:
    Callback()
        : ref(0)
    {}

    Callback(std::function<T> &callback)
        : ref(0)
        , callback(callback)
    {}
    Callback(const Callback<T> &other)
        : ref(other.ref.load())
        , callback(other.callback)
    {}
    Callback &operator=(const Callback<T> &other)
    {
        ref.store(other.load());
        callback = other.callback;
    }

    void inc() { ++ref; }
    void dec() { --ref; }

    std::atomic<int> ref;
    std::function<T> callback;
};

template<typename T>
struct CallbackContainer
{
public:
    const RefCounter<T> addCallback(std::function<T> &callback)
    {
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (vec[i].ref.load() == 0) {
                vec[i].callback = callback;
                return RefCounter<T>(i, this);
            }
        }
        vec.push_back(Callback<T>(callback));
        return RefCounter<T>(vec.size() - 1, this);
    }

    template<typename ...Ts>
    void invokeCallbacks(Ts&...args)
    {
        for (auto &callbackHandler : vec)
        {
            if (callbackHandler.ref.load())
            {
                callbackHandler.callback(args...);
            }
        }
    }
    void inc(size_t index)
    {
        assert(index < vec.size());
        ++vec[index].ref;
    }
    void dec(size_t index)
    {
        assert(index < vec.size());
        assert(vec[index].ref.load() != 0);
        --vec[index].ref;
    }
private:
    std::vector<Callback<T>> vec;
};

template<typename T>
inline void RefCounter<T>::inc()
{
    if(callbackContainer)
        callbackContainer->inc(index);
}

template<typename T>
inline void RefCounter<T>::dec()
{
    if(callbackContainer)
        callbackContainer->dec(index);
}
