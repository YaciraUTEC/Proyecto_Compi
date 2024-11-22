fun main() {
    var x: Int
    val y: Int

    x = 5
    y = 10
    
    if (x > y) {
        println(x)
        x = 20
    } else {
        x = 20
        println(y)
    }
    println(x)
}
