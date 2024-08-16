'use strict';

document.querySelector("h1").addEventListener("click", function() {
	this.style.backgroundColor = this.style.backgroundColor == "lightcoral" ? "transparent" : "lightcoral"
})