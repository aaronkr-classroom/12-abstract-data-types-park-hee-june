#ifndef GUARD_H_VEC
#define GUARD_H_VEC

#include <memory>//메모리 allocator
#include <cstddef>//size_t, ptrdiff_t
#include <algorithm>//max 함수

using namespace std;

//Vec 클래스 추상 데이터 타입
template <class T> class Vec {
public:
    //인터페이스
    //타입 정의문
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    //생성자
    Vec() { create(); }
    explicit Vec(size_type n, const T& val = T()) {
        create(n, val);
    }

    //반복생성자
    Vec(const Vec& v) { create(v.begin(), v.end()); }
    Vec& operator=(const Vec&);//선언문 (이렇게 하겠습니다)

    //소멸자
    ~Vec() { uncreate(); }

    //추가하는 함수 
    bool empty() const { return data == avail; } // 벡터가 비어 있는지 여부 확인
    void clear() { uncreate(); } // 벡터의 모든 요소 제거

    //벡터 크기와 인덱스에 관한 새로운 함수
    size_type size() const { return avail - data; }
    T& operator[](size_type i) { return data[i]; }
    const T& operator[](size_type i) const { return data[i]; }

    //반복자를 반환하는 새로운함수
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    //요소 추가하는 함수(메모리를 채우면 확장합니다)
    void push_back(const T& val) {
        if (avail == limit)//필요하다면 저장공간 확보
            grow();

        unchecked_append(val);//새로운 요소추가
    }
private:
    iterator data;// Vec의 첫번째 요소를 가르키는 포인터
    iterator avail;//마지막으로 만든 요소의 다음을 가르키는 포인터
    iterator limit;//사용할수있는 마지막요소의 다음을 가르키는 포인터

    allocator<T> alloc; //메모리를 할당하는 객체
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);

    void uncreate();

    void grow();
    void unchecked_append(const T&);
};

//생성자  함수들
template <class T> void Vec<T>::create() {
    data = avail = limit = 0;
}

template <class T> void Vec<T>::create(size_type n, const T& val) {
    data = alloc.allocate(n);
    limit = avail = data + n;
    uninitialized_fill(data, limit, val);//allocator 비멤버 함수
}

template <class T> void Vec<T>::create(const_iterator i, const_iterator j) {
    data = alloc.allocate(j - i);
    limit = avail = uninitialized_copy(i, j, data);//allocator 비멤버 함수
}



template <class T> void Vec<T>::uncreate() {
    if (data) {
        // 역순으로 요소들을 파괴합니다.
        iterator it = avail;
        while (it != data)
            alloc.destroy(--it);

        // 할당된 메모리를 해제합니다.
        alloc.deallocate(data, limit - data);
    }
    //비어있는 Vec을 가르키도록 포인터를 재설정
    data = avail = limit = nullptr;
}
template <class T> void Vec<T>::grow() {
    // 메모리 공간을 확장하는 함수
    size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

    //새로운 저장공간을 할당하고 기존 요소들을 새로운 저장공간에 복사
    iterator new_data = alloc.allocate(new_size);//메모리 확보하기
    iterator new_avail = uninitialized_copy(data, avail, new_data);

    uncreate();

    data = new_data;
    avail = new_avail;
    limit = data + new_size;
}

// 새로운 요소를 추가하는 함수
template <class T> void Vec<T>::unchecked_append(const T& val) {
    alloc.construct(avail++, val);
}

//= 피연산자 정의
template <class T> Vec<T>& Vec<T>::operator=(const Vec& rhs) {
    //자가할당여부 확인
    if (&rhs != this) {
        //좌변과 우변이 지닌 배열이 점유하는 메모리를 해제
        uncreate();

        //우변이 지닌 요소를 좌변으로 복사
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

#endif 