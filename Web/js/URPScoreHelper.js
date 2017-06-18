
function show_loading() {
	$.showPreloader('���Ժ�');
	setTimeout(function () {
		$.hidePreloader();
	}, 10000);
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
function removeCell(){
	if(document.getElementById("user") != undefined) {
	����for(var i=0;i<document.getElementById("user").rows.length;i++){
			document.getElementById("user").rows[i].deleteCell(0);
			document.getElementById("user").rows[i].deleteCell(0);
	��������document.getElementById("user").rows[i].deleteCell(1);
	����}
		if(getQueryString("order") == "passed")
		{
			document.getElementsByClassName("sortable")[0].style.width="60%";
			return;
		}
		if(getQueryString("order") == "byplan")
		{
			document.getElementsByClassName("sortable")[0].style.width="45%";
			document.getElementsByClassName("sortable")[4].style.width="25%";
			return;
		}
		if(getQueryString("order") == "failed")
		{
			document.getElementsByClassName("sortable")[0].style.width="25%";
			document.getElementsByClassName("sortable")[1].style.width="5%";
			document.getElementsByClassName("sortable")[2].style.width="5%";
			document.getElementsByClassName("sortable")[3].style.width="5%";
			return;
		}
	}
}
$(function () {
	$(document).on("click", "a:not(.except)", function(e) {
		show_loading();
	});
		$(document).on("click", "#i_submit", function(e) {
				document.oncontextmenu=new Function("event.returnValue=false;");
				   var r1 = document.getElementById("i_xh").value;
				   var r2 = document.getElementById("i_mm").value;
				   var r3 = document.getElementById("i_yzm").value;
				if(r1=="")
				{
					$.toast("(��o��) ѧ�Ż�û���ء�");
					return false;
				}
				if(r2=="")
				{
					$.toast("(��o��) ���뻹û���ء�");
					return false;
				}
				if(r3=="")
				{
					$.toast("(��o��) ��֤�뻹û���ء�");
					return false;
				}
				show_logging();
				function show_logging() {
					$.showPreloader('O(��_��)O ���ڵ�¼');
					setTimeout(function () {
						$.hidePreloader();
					}, 10000);
				 }
		});
		
		$(document).on("pageInit", "", function(e, id, page) {
			//���廭����ߺ����ɵ�ĸ���
			var WIDTH = window.innerWidth - 25, HEIGHT = window.innerHeight - 100, POINT = 20;
			
			var canvas = document.getElementById('i_canvas');
			canvas.width = WIDTH,
			canvas.height = HEIGHT;
			var context = canvas.getContext('2d');
			context.strokeStyle = 'rgba(0,0,0,0.2)',
			context.strokeWidth = 1,
			context.fillStyle = 'rgba(0,0,0,0.1)';
			var circleArr = [];

			//��������ʼxy���꣬����xy���꣬����͸����
			function Line (x, y, _x, _y, o) {
				this.beginX = x,
				this.beginY = y,
				this.closeX = _x,
				this.closeY = _y,
				this.o = o;
			}
			//�㣺Բ��xy���꣬�뾶��ÿ֡�ƶ�xy�ľ���
			function Circle (x, y, r, moveX, moveY) {
				this.x = x,
				this.y = y,
				this.r = r,
				this.moveX = moveX,
				this.moveY = moveY;
			}
			//����max��min֮��������
			function num (max, _min) {
				var min = arguments[1] || 0;
				return Math.floor(Math.random()*(max-min+1)+min);
			}
			// ����ԭ��
			function drawCricle (cxt, x, y, r, moveX, moveY) {
				var circle = new Circle(x, y, r, moveX, moveY)
				cxt.beginPath()
				cxt.arc(circle.x, circle.y, circle.r, 0, 2*Math.PI)
				cxt.closePath()
				cxt.fill();
				return circle;
			}
			//��������
			function drawLine (cxt, x, y, _x, _y, o) {
				var line = new Line(x, y, _x, _y, o)
				cxt.beginPath()
				cxt.strokeStyle = 'rgba(0,0,0,'+ o +')'
				cxt.moveTo(line.beginX, line.beginY)
				cxt.lineTo(line.closeX, line.closeY)
				cxt.closePath()
				cxt.stroke();

			}
			//��ʼ������ԭ��
			function init () {
				circleArr = [];
				for (var i = 0; i < POINT; i++) {
					circleArr.push(drawCricle(context, num(WIDTH), num(HEIGHT), num(15, 2), num(10, -10)/40, num(10, -10)/40));
				}
				draw();
			}

			//ÿ֡����
			function draw () {
				context.clearRect(0,0,canvas.width, canvas.height);
				for (var i = 0; i < POINT; i++) {
					drawCricle(context, circleArr[i].x, circleArr[i].y, circleArr[i].r);
				}
				for (var i = 0; i < POINT; i++) {
					for (var j = 0; j < POINT; j++) {
						if (i + j < POINT) {
							var A = Math.abs(circleArr[i+j].x - circleArr[i].x),
								B = Math.abs(circleArr[i+j].y - circleArr[i].y);
							var lineLength = Math.sqrt(A*A + B*B);
							var C = 1/lineLength*7-0.009;
							var lineOpacity = C > 0.03 ? 0.03 : C;
							if (lineOpacity > 0) {
								drawLine(context, circleArr[i].x, circleArr[i].y, circleArr[i+j].x, circleArr[i+j].y, lineOpacity);
							}
						}
					}
				}
			}
			//����ִ��
				init();
				setInterval(function () {
					for (var i = 0; i < POINT; i++) {
						var cir = circleArr[i];
						cir.x += cir.moveX;
						cir.y += cir.moveY;
						if (cir.x > WIDTH) cir.x = 0;
						else if (cir.x < 0) cir.x = WIDTH;
						if (cir.y > HEIGHT) cir.y = 0;
						else if (cir.y < 0) cir.y = HEIGHT;
						
					}
					draw();
				}, 48);
		});
		$.init();
});
$.config = {router: false}
function show_about() {
	var r1 = document.getElementById("about").value;
	$.alert(r1);
}
function check() {
	var r1 = document.getElementById("i_xhs").value;
	if(r1=="")
	{
		$.toast("(��o��) �ܵ�����Щʲô�ɣ�");
		return false;
	}
	$.showPreloader('O(��_��)O ���Ժ�');
	setTimeout(function () {
		$.hidePreloader();
	}, 10000);
	return true;
}