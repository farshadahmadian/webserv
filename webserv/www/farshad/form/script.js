"use strict";

const fileInput = document.querySelector(".file-to-delete");
const deleteBtn = document.querySelector(".delete");

fileInput.placeholder = "Path to file";

const sendDeleteRequest = function (event) {
  event.preventDefault();
  console.log(fileInput.value);

  fetch("/delete", {
    method: "DELETE",
    headers: {
      "Content-Type":
        "multipart/form-data; boundary=----WebKitFormBoundaryH3mvRLHMNYY29Zsu\r\n",
    },
    body: `------WebKitFormBoundaryH3mvRLHMNYY29Zsu\r\nContent-Disposition: form-data; name="_method"\r\n\r\ndelete\r\n------WebKitFormBoundaryH3mvRLHMNYY29Zsu\r\nContent-Disposition: form-data; name="name"\r\n\r\n${fileInput.value}\r\n------WebKitFormBoundaryH3mvRLHMNYY29Zsu--\r\n\r\n`,
  })
    .then((response) => {
      console.log(response);
      return response.text();
    })
    .then((result) => {
      console.log(result),
        // document.open();
        // document.write(result);
        // document.close();
        (document.querySelector("html").innerHTML = result);
      history.pushState({}, "", "/delete");
      // window.location.reload();
      // window.location.replace();
      // window.location.href = "/delete";
    })
    .catch((err) => console.log("ERROR: ", err));
  fileInput.value = "";
};

deleteBtn.addEventListener("click", sendDeleteRequest);

window.addEventListener("popstate", () => window.location.reload());
