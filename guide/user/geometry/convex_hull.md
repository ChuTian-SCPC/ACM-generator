## 凸包(ConvexHull)

`ConvexHull<T>`表示一个凸包，`T`是点的坐标类型，只能为有符号整型或者浮点型。

#### 成员变量

##### 含义

```cpp
int node_count;                // 点的数量
std::vector<Point<T>> points;  // 点
int max_try;                   // 尝试生成上限
T x_left_limit;                // x坐标的下界
T x_right_limit;               // x坐标的上界
T y_left_limit;                // y坐标的下界
T y_right_limit;               // y坐标的上界
bool output_node_count;        // 是否输出点数
```

`max_try`是因为对于特定的范围难以判断能够生成的上界，所以有生成失败的情况，最多会尝试生成`max_try`次，如果生成不了则会报告错误。

##### 获取

变量获取分成可以修改（引用）和`const`的版本。

```cpp
int node_count() const;
std::vector<Point<T>> points() const;
int max_try() const;
T x_left_limit() const;
T x_right_limit() const;
T y_left_limit() const;
T y_right_limit() const;

int& node_count_ref();
std::vector<Point<T>>& points_ref();
int& max_try_ref();
T& x_left_limit_ref();
T& x_right_limit_ref();
T& y_left_limit_ref();
T& y_right_limit_ref();
```

##### 修改

```cpp
void set_node_count(int node_count);
void set_max_try(int max_try);
void set_x_left_limit(T x_left_limit);
void set_x_right_limit(T x_right_limit);
void set_y_left_limit(T y_left_limit);
void set_y_right_limit(T y_right_limit);
void set_x_limit(T x_left_limit, T x_right_limit);
void set_y_limit(T y_left_limit, T y_right_limit);
void set_xy_limit(T left, T right);
void set_output_node_count(bool output_node_count);
void set_x_limit(const char* format, ...);
void set_y_limit(const char* format, ...);
void set_xy_limit(const char* format, ...);
```

其中设置范围的传参是字符串形式的参考[点](./point.md)里面对于范围字符串的描述。

#### 输出

可以通过`void set_output_default()`设置默认的输出格式。

默认的输出的格式是：

```
点数
x1 y1
x2 y2
……
xn yn
```

`output_node_count`控制是否输出点数。

也可以通过`void set_output(std::function<void(std::ostream&, const ConvexHull<T>&)>)`自行定义输出方式。

树的输出可以通过重载的`std::ostream& <<`输出，也可以通过`println`输出。区别在于`std::ostream& <<`不会换行，而`println`会换行，即等价于`std::cout<<graph<<std::endl;`。

#### 使用方法

它的构造函数是：

```cpp
ConvexHull(
    int node_count = 1, 
    T x_left_limit = 0, 
    T x_right_limit = 0, 
    T y_left_limit = 0, 
    T y_right_limit = 0
);
```

在设置好各个属性后，使用函数`void gen()`完成生成一个凸包。

详见[示例代码](../../../examples/rand_convex_hull.cpp)

[返回目录](../../home.md)