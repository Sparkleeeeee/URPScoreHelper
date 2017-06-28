function createxmlHttpRequest() {
	if (window.ActiveXObject) {
		return new ActiveXObject("Microsoft.XMLHTTP");
	} else if (window.XMLHttpRequest) {
		return new XMLHttpRequest();
	}
}

function convertData(data) {
	if (typeof data === 'object') {
		var convertResult = "";
		for (var c in data) {
			convertResult += c + "=" + data[c] + "&";
		}
		convertResult = convertResult.substring(0, convertResult.length - 1);
		return convertResult;
	} else {
		return data;
	}
}

function ajax() {
	var ajaxData = {
		type: arguments[0].type || "GET",
		url: arguments[0].url || "",
		async: arguments[0].async || "true",
		data: arguments[0].data || null,
		dataType: arguments[0].dataType || "text",
		contentType: arguments[0].contentType || "application/x-www-form-urlencoded",
		beforeSend: arguments[0].beforeSend ||
		function() {},
		success: arguments[0].success ||
		function() {},
		error: arguments[0].error ||
		function() {}
	}
	ajaxData.beforeSend();
	var xhr = createxmlHttpRequest();
	xhr.responseType = ajaxData.dataType;
	xhr.open(ajaxData.type, ajaxData.url, ajaxData.async);
	xhr.setRequestHeader("Content-Type", ajaxData.contentType);
	xhr.send(convertData(ajaxData.data));
	xhr.onreadystatechange = function() {
		if (xhr.readyState == 4) {
			if (xhr.status == 200) {
				ajaxData.success(xhr.responseText)
			} else {
				ajaxData.error(xhr.responseText)
			}
		}
	}
}

function get_captcha() {
	var obj = document.getElementById("login_captcha");
	if(obj == undefined)
		return;
	ajax({
		type: "GET",
		url: "captcha.cgi",
		beforeSend: function() {
			obj.src = "img/loading.gif";
		},
		success: function(msg) {
			obj.src = msg;
			ocr_captcha(msg);
		},
		error: function(msg) {
			obj.src = "img/refresh.png";
			$.toast(msg);
		}
	})
}

function ocr_captcha(dataURI) {
	ajax({
		type: "POST",
		url: "https://api.iedon.com/captcha_ocr/ocr.php",
		data:{"pic":dataURI}, 
		success: function(msg) {
			if(msg != null && msg.length != 0) {
				if(msg == '__ERROR')
				{
					get_captcha();
					return;
				}
				var patrn=/^(\w){4,4}$/;
				if (!patrn.exec(msg))
				{
					get_captcha();
					return;
				}
				msg.replace("l", "1");
				msg.replace("I", "1");
				msg.replace("O", "0");
				document.getElementById("i_yzm").value = msg;
			}
		},
	})
}
function getQueryString(name) {
	var i;
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", i); // ƥ��Ŀ�����
    var result = window.location.search.substr(1).match(reg);  // ��querystringƥ��Ŀ�����
    if (result != null) {
        return decodeURIComponent(result[2]);
    } else {
        return null;
    }
}
function autofill() {
	obj = document.getElementById("i_xh");
	if(obj.value=="")
	{
		$.toast("ѧ�Ż�û����","cancel");
		return false;
	}
	$.toptip("���Զ��������룬������ѧ����ͬ������", 3000, 'success');
	document.getElementById("i_mm").value = obj.value;
}
function checkRows() {
	var rows = document.getElementById("i_xh").value.split(/\r?\n|\r/).length;
	document.getElementById("i_xhhs").innerHTML = rows;
}
function getcharnum() {
	var nums = document.getElementById("i_jxpg").value.length;
	document.getElementById("i_xhhs").innerHTML = nums;
}
function logout() {
		$.confirm("ȷ��Ҫ�˳�ϵͳ��", function() {
			$.showLoading("���ڵǳ�...");
			window.location.href = "index.cgi?act=logout";
		}, function() {
		});
}
function releaseAssoc(id) {
	$.confirm("ȷ��Ҫ���ѧ����QQ�ŵĹ�����", function() {
		$.showLoading("���ڽ��...");
		window.location.href = "OAuth2Assoc.cgi?release="+id;
	}, function() {
	});
}
function check_password() {
		var r1 = document.getElementById("i_xhs").value;
		if(r1=="")
		{
			$.toast("������������","cancel");
			return false;
		}
		var patrn=/^(\w){6,12}$/;
		if (!patrn.exec(r1)){
			$.toast("ֻ������6-12����ĸ�����֡��»���", "text");
			return false;
		}
		if(!document.getElementById("i_chk").checked){
			$.toast("�����ұ߿�����ȷ���������� :-)", "text");
			return false;
		}
		$.showLoading("�����޸�");
		return true;
}
$(function () {
	get_captcha();
		$(document).on("click", "#i_submit", function(e) {
				document.oncontextmenu=new Function("event.returnValue=false;");
				   var r1 = document.getElementById("i_xh");
				   var r2 = document.getElementById("i_mm");
				   var r3 = document.getElementById("i_yzm");
				   var r4 = document.getElementById("weuiAgree");
				   var r5 = document.getElementById("i_jxpj");
				if(r1 != undefined && r1.value=="")
				{
					$.toast("ѧ�Ż�û����","cancel");
					return false;
				}
				if(r2 != undefined && r2.value=="")
				{
					$.toast("���뻹û����","cancel");
					return false;
				}
				if(r3 != undefined && r3.value=="")
				{
					$.toast("��֤�뻹û����","cancel");
					return false;
				}
				if(r4 != undefined && !r4.checked)
				{
					$.toast("����ͬ������Ŷ","cancel");
					return false;
				}
				if(r5 != undefined && r5.value=="")
				{
					$.toast("ûд�������۰�","cancel");
					return false;
				}
				$.showLoading("���Ժ�");
		});
});