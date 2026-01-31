package main

import (
  "net/http"
  "log"
)

func NetstarHTTPRedirect(responseWriter http.ResponseWriter, request *http.Request) {
  responseWriter.Header().Set("Location", "http://192.168.18.28:8080"+request.URL.RequestURI())
  responseWriter.Header().Set("Connection", "close")
  responseWriter.WriteHeader(http.StatusTemporaryRedirect)
}

func main() {
 	log.Println("[ netstar-http-redirect ] started")

	go func() {
    err := http.ListenAndServe(
      ":8081",
      http.HandlerFunc(NetstarHTTPRedirect),
    )
    if err != nil {
      log.Fatal(err)
    }
  }()

  err := http.ListenAndServeTLS(
    ":8082",
    "share/server/certificates/cert.pem",
    "share/server/certificates/key.pem",
    http.HandlerFunc(NetstarHTTPRedirect),
  )
  if err != nil {
    log.Fatal(err)
  }
}
