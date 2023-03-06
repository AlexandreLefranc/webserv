async function go_GET() {
	const path = document.getElementById("get_path").value;
	let url = "";
	if (path) {
		url = "http://localhost:8080/" + path;
		console.log(url);
	}
	else {
		console.log("empty path.");
		return ;
	}
	try {
		const response = await fetch(url);
		// const data = await response.json();
	} catch (error) {
		console.error(error);
		return ;
	}
	document.getElementById("status_box").value = response.status;
	// document.getElementById("msg_box").value = response.
}

async function go_POST() {
	const path = document.getElementById("post_path").value;
	const body = document.getElementById("post_body").value;
	const file_path = document.getElementById("file_path").value;
}