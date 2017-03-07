$(function () {
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
				show_querying();
				function show_querying() {
					$.showPreloader('O(��_��)O ���ڲ�ѯ');
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
				}, 16);
		});
		
		$.init();
});
function show_loading() {
	$.showPreloader('���Ժ�');
	setTimeout(function () {
		$.hidePreloader();
	}, 10000);
}
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
	$.showPreloader('O(��_��)O ���ڲ�ѯ');
	setTimeout(function () {
		$.hidePreloader();
	}, 10000);
	return true;
}