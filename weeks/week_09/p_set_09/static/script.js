window.setTimeout(function () {
	try {
		const alert = document.querySelector(".alert-dismissible");
		const btn = alert.querySelector("button");
		btn.click();
	} catch (TypeError) {
		console.log("No flask messages.");
	}
}, 3000);
