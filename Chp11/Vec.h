#ifndef GUARD_H_VEC
#define GUARD_H_VEC

#include <memory>//�޸� allocator
#include <cstddef>//size_t, ptrdiff_t
#include <algorithm>//max �Լ�

using namespace std;

//Vec Ŭ���� �߻� ������ Ÿ��
template <class T> class Vec {
public:
    //�������̽�
    //Ÿ�� ���ǹ�
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    //������
    Vec() { create(); }
    explicit Vec(size_type n, const T& val = T()) {
        create(n, val);
    }

    //�ݺ�������
    Vec(const Vec& v) { create(v.begin(), v.end()); }
    Vec& operator=(const Vec&);//���� (�̷��� �ϰڽ��ϴ�)

    //�Ҹ���
    ~Vec() { uncreate(); }

    //�߰��ϴ� �Լ� 
    bool empty() const { return data == avail; } // ���Ͱ� ��� �ִ��� ���� Ȯ��
    void clear() { uncreate(); } // ������ ��� ��� ����

    //���� ũ��� �ε����� ���� ���ο� �Լ�
    size_type size() const { return avail - data; }
    T& operator[](size_type i) { return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    //�ݺ��ڸ� ��ȯ�ϴ� ���ο��Լ�
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    //��� �߰��ϴ� �Լ�(�޸𸮸� ä��� Ȯ���մϴ�)
    void push_back(const T& val) {
        if (avail == limit)//�ʿ��ϴٸ� ������� Ȯ��
            grow();

        unchecked_append(val);//���ο� ����߰�
    }
private:
    iterator data;// Vec�� ù��° ��Ҹ� ����Ű�� ������
    iterator avail;//���������� ���� ����� ������ ����Ű�� ������
    iterator limit;//����Ҽ��ִ� ����������� ������ ����Ű�� ������

    allocator<T> alloc; //�޸𸮸� �Ҵ��ϴ� ��ü
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);

    void uncreate();

    void grow();
    void unchecked_append(const T&);
};

//������  �Լ���
template <class T> void Vec<T>::create() {
    data = avail = limit = 0;
}

template <class T> void Vec<T>::create(size_type n, const T& val) {
    data = alloc.allocate(n);
    limit = avail = data + n;
    uninitialized_fill(data, limit, val);//allocator ���� �Լ�
}

template <class T> void Vec<T>::create(const_iterator i, const_iterator j) {
    data = alloc.allocate(j - i);
    limit = avail = uninitialized_copy(i, j, data);//allocator ���� �Լ�
}



template <class T> void Vec<T>::uncreate() {
    if (data) {
        // �������� ��ҵ��� �ı��մϴ�.
        iterator it = avail;
        while (it != data)
            alloc.destroy(--it);

        // �Ҵ�� �޸𸮸� �����մϴ�.
        alloc.deallocate(data, limit - data);
    }
    //����ִ� Vec�� ����Ű���� �����͸� �缳��
    data = avail = limit = nullptr;
}
template <class T> void Vec<T>::grow() {
    // �޸� ������ Ȯ���ϴ� �Լ�
    size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

    //���ο� ��������� �Ҵ��ϰ� ���� ��ҵ��� ���ο� ��������� ����
    iterator new_data = alloc.allocate(new_size);//�޸� Ȯ���ϱ�
    iterator new_avail = uninitialized_copy(data, avail, new_data);

    uncreate();

    data = new_data;
    avail = new_avail;
    limit = data + new_size;
}

// ���ο� ��Ҹ� �߰��ϴ� �Լ�
template <class T> void Vec<T>::unchecked_append(const T& val) {
    alloc.construct(avail++, val);
}

//= �ǿ����� ����
template <class T> Vec<T>& Vec<T>::operator=(const Vec& rhs) {
    //�ڰ��Ҵ翩�� Ȯ��
    if (&rhs != this) {
        //�º��� �캯�� ���� �迭�� �����ϴ� �޸𸮸� ����
        uncreate();

        //�캯�� ���� ��Ҹ� �º����� ����
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

#endif 