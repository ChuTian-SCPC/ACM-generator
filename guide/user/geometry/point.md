## 点(Point)

`Point<T>`表示一个二维点，`T`是坐标类型，只能为有符号整型或者浮点型。

它的构造函数是:

```cpp
Point(); // 默认x，y为0
Point(T x,T y);
```

#### 成员变量

`x,y`可以通过以下方式获取：
```cpp
T x();
T y();
T& x_ref();
T& y_ref();

T& operator[](int idx);
T& operator[](char c);
T& operator[](std::string s);
```
对于函数`operator []`，参数为`0`,`x`,`X`的时候获取x的值，其余为y的值。

#### 输出

可以通过`void set_output_default()`设置默认的输出格式。

默认的输出的格式是：`x y`。

也可以通过`void set_output(std::function<void(std::ostream&, const Point<T>&)>)`自行定义输出方式。

树的输出可以通过重载的`std::ostream& <<`输出，也可以通过`println`输出。区别在于`std::ostream& <<`不会换行，而`println`会换行，即等价于`std::cout<<point<<std::endl;`。

#### 随机

`Point`有`rand`函数：

1. `void rand(T x_left, T x_right, T y_left, T y_right)`

    参数是`x`的范围和`y`的范围。

2. `void rand(const char* format, ...)`

    参数是一个字符串表示`x`的范围和`y`的范围，用开闭区间和逗号表示。可以在范围前面加上`xXyY`表示后面的范围是标识哪一个的，如果不标则第一个区间表示`x`的范围，第二个区间表示`y`的范围。可以只有一个区间，表示`x`和`y`的范围都是这个区间。

详见[示例代码](../../../examples/rand_point1.cpp)

还有函数可以获得随机的`Point<T>`：

1. `Point<T> rand_point(T x_left, T x_right, T y_left, T y_right)`
   
    参数是`x`的范围和`y`的范围。

2. `Point<T> rand_point(T left, T right)`

    参数是一个范围，表示`x`和`y`的范围都是这个。

3. `Point<T> rand_point(const char* format, ...)`

    参数是一个字符串表示`x`的范围和`y`的范围，详细格式见上述。

详见[示例代码](../../../examples/rand_point2.cpp)

[返回目录](../../home.md)