function get_captcha() {
	var obj = document.getElementById("login_captcha");
	if(obj == undefined)
		return;
	$.ajax({
		type: "GET",
		url: "/captcha.fcgi",
		dataType: "text",
		cache: false,
		beforeSend: function() {
			obj.src = "/img/loading.gif";
		},
		success: function(data) {
			if(data == "LOGGED-IN")
			{
				$.toast("已登录过, 正在跳转", "text");
				window.location.href = "/main.fcgi";
			}
			else if(data == "REQUEST-FAILED")
			{
				$.toast("验证码失踪了！学院系统可能发生故障", "text");
				obj.src = "/img/refresh.png";
			}
			else
			{
				obj.src = data;
			}
		},
		error: function() {
			obj.src = "/img/refresh.png";
		}
	})
}
function get_avatar() {
	var obj = document.getElementsByClassName("i_user-photo")[0];
	if(obj == undefined)
		return;
	$.ajax({
		type: "GET",
		url: "/avatar.fcgi",
		dataType: "text",
		success: function(data) {
			if(data == "LOGGED-OUT")
			{
				$.toast("尚未登录，请登录！", "text");
				window.location.href = "/index.fcgi";
			}
			else
			{
				obj.src = data;
			}
		},
		error: function() {
			$.toast("照片飘到火星啦，请刷新重试。", "text");
		}
	})
}
function autoreset() {
	obj1 = document.getElementById("i_xh");
	obj2 = document.getElementById("i_mm");
	obj1.value = "";
	obj2.value = "";
	obj1.focus();
}
function autofill() {
	obj = document.getElementById("i_xh");
	if(obj.value=="")
	{
		$.toast("学号还没输呢","cancel");
		return false;
	}
	$.toptip("已自动填入密码，密码与学号相同方可用", 3000, 'success');
	document.getElementById("i_mm").value = obj.value;
	document.getElementById("i_yzm").focus();
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
	$.confirm("确认要退出系统吗？", function() {
		window.location.href = "/index.fcgi?act=logout";
	}, function() {
	});
}
function releaseAssoc(id) {
	$.confirm("确定要解除学号与QQ号的关联吗？", function() {
		window.location.href = "/OAuth2Assoc.fcgi?release=" + id;
	}, function() {
	});
}
function check_password() {
	var r1 = document.getElementById("i_xhs").value;
	if(r1=="")
	{
		$.toast("请输入新密码","cancel");
		return false;
	}
	var patrn=/^(\w){6,12}$/;
	if (!patrn.exec(r1)){
		$.toast("只能输入6-12个字母、数字、下划线", "text");
		return false;
	}
	if(!document.getElementById("i_chk").checked){
		$.toast("滑动右边开关来确认输入无误 :-)", "text");
		return false;
	}
	return true;
}
function getQueryString(name) {
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i");
    var r = window.location.search.substr(1).match(reg);
    if (r != null) {
        return unescape(r[2]);
    }
    return null;
}
function adjust_form() {
	if(window.location.pathname == "/") {
		return;
	}
	r1 = document.getElementById("i_xh");
	r2 = document.getElementById("i_mm");
	if(r1 == undefined || r2 == undefined) {
		return;
	}
	if(r1.value != "") {
		document.getElementsByClassName("weui-cell weui-cell_vcode")[0].style.display = "none";
	}
	if(r2.value != "") {
		document.getElementsByClassName("weui-cell weui-cell_vcode")[1].style.display = "none";
	}
}
$(function () {
	adjust_form();
	get_captcha();
	get_avatar();
	var r1 = document.getElementById("i_xh");
	var r2 = document.getElementById("i_mm");
	var r3 = document.getElementById("i_yzm");
	if(r1 != undefined && r1.value == "")
	{
		r1.focus();
	}
	if(r1 != undefined && r1.value != "" && r2 != undefined && r2.value == "")
	{
		r2.focus();
	}
	if(r1 != undefined && r1.value != "" && r2 != undefined && r2.value != "")
	{
		r3.focus();
	}
	$(document).on("click", "#i_submit", function(e) {
		document.oncontextmenu=new Function("event.returnValue=false;");
		var r4 = document.getElementById("weuiAgree");
		var r5 = document.getElementById("i_jxpj");
		if(r1 != undefined && r1.value=="")
		{
			$.toast("学号还没输呢","cancel");
			return false;
		}
		if(r2 != undefined && r2.value=="")
		{
			$.toast("密码还没输呢","cancel");
			return false;
		}
		if(r3 != undefined && r3.value=="")
		{
			$.toast("验证码还没输呢","cancel");
			return false;
		}
		if(r4 != undefined && !r4.checked)
		{
			$.toast("必须同意条款哦","cancel");
			return false;
		}
		if(r5 != undefined && r5.value=="")
		{
			$.toast("没写主观评价啊","cancel");
			return false;
		}
	});
});