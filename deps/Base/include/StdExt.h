template <typename T>
void erase_back(vector<T>& v, size_t i)
{
    swap(v[i], v.back());
    v.pop_back();
}

template <typename T>
bool try_find(vector<T>& v, T item, size_t* outIndex)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        T _item = v[i];
        if (_item == item)
        {
            *outIndex = i;
            return true;
        }
    }

    return false;
}

template <typename T>
bool try_remove(vector<T>& v, T item)
{
    size_t index;
    if (try_search(v, item, &index))
    {
        erase_back(v, index);
        return true;
    }
    return false;
}
