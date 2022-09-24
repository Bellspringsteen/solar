package main

import (
    "fmt"
    "log"
    "net/http"
   // "strconv"
    "sync"
    "time"
)


// http://localhost:8081/solar?key=21

// http://localhost:8081/getSolar

var solarText = ""
var mutex = &sync.Mutex{}

func saveSolar(w http.ResponseWriter, r *http.Request) {
	keys, ok := r.URL.Query()["key"]

	if !ok || len(keys[0]) < 1 {
		log.Println("Url Param 'key' is missing")
		return
	}

	key := keys[0]
	log.Println("Url Param 'key' is: " + string(key))

	mutex.Lock()
	//solar,_ :=strconv.Atoi(string(key))
	timeSave := time.Unix(time.Now().Unix(), 0)
	solarText = solarText+"\n "+timeSave.Format("2006-01-02 15:04:05")
        mutex.Unlock()
}

func main() {

    http.HandleFunc("/solar", saveSolar)

    http.HandleFunc("/getSolar", func(w http.ResponseWriter, r *http.Request) {
        fmt.Fprintf(w, " "+solarText)
    })

    log.Fatal(http.ListenAndServe(":8081", nil))

}
