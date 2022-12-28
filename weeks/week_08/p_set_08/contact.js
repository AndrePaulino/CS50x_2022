const contactFormContainer = document.getElementById("contact-container");

contactFormContainer.addEventListener("submit", (e) => {
	const message = "Thank you for your message!";
	const wrapper = document.createElement("div");
	wrapper.innerHTML =
		'<div class="alert alert-success' +
		' alert-dismissible" role="alert">' +
		message +
		'<button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button></div>';
	contactFormContainer.append(wrapper);

	e.preventDefault();
});
