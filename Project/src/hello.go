package main
//调取函数包
import "fmt"

//常量定义
const p = 33
//全局变量的声明和赋值
var name = "lhl"
//一般类型声明
type newType int
//结构的声明
type gopher struct{}
//函数声明(函数首字母小写为私有函数，首字母大写为共有函数)
func getID(){}
//由main函数作为程序入口点启动
const (
    i=1<<iota
    j=3<<iota
    k
    l
)
func main() {
    fmt.Println("Hello, word")
    fmt.Println("菜鸟教程：runoob.com")
    fmt.Println("i=",i)
    fmt.Println("j=",j)
    fmt.Println("k=",k)
    fmt.Println("l=",l)
}