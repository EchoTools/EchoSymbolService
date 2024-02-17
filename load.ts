const endpoint = "http://127.0.0.1:8080/";

setInterval(() => {
  const randomString = Math.random().toString(36).substring(7);
  fetch(endpoint + randomString);
}, 1);
