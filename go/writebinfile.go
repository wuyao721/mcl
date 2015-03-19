// Write file with io.WriteString
// from https://www.socketloop.com/tutorials/golang-write-file-io-writestring
package main

import (
    "os"
    "io"
    "fmt"
)

func main() {

    filename := "output.txt"

    file, err := os.Create(filename)

    if err != nil {
        fmt.Println(err)
    }

    fmt.Println(" Write to file : " + filename)

    n, err := io.WriteString(file, " Hello World !")

    if err != nil {
        fmt.Println(n, err)
    }

    file.Close()
}
