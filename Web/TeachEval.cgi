<div class="page-group">
	<div class="page">
		<header class="bar bar-nav">
		<button class="button button-link button-nav pull-left">
		<span class="icon icon-left"></span><a href="main.cgi">����</a></button>
		<h1 class="title">һ����ѧ����</h1>
		</header>
		<nav class="bar bar-tab">
		<a class="tab-item external active" href="main.cgi">
		<span class="icon icon-star"></span>
		<span class="tab-label">һ����ѧ����</span>
		</a>
		</nav>
		<canvas id="i_canvas" style="position:absolute;top:0;left:0;right:0;bottom:0"></canvas>
		<div class="content" style="opacity: 0.75;z-index:9999">
			  %s
			  <form action="TeachEval.cgi?act=Evaluate" method="post" style="display:%s">
				  <div class="list-block">
					<ul style="opacity: 0.75;z-index:9999">
					  <li>
						<div class="item-content">
						  <div class="item-media">
							<i class="icon icon-form-name"></i>
						  </div>
						  <div class="item-inner">
							<div class="item-title label">��������</div>
							<div class="item-input">
							  <textarea id="i_xhs" name="nr" type="text" placeholder="��ʦ��ô���࣬�������Ǻ����� :-)">��ʦ�����Ч���ڿ���ϸ�����û�Ծ��ÿ�ڿζ����ջ񣬸���ʦ����ޣ�</textarea></div>
						  </div>
						</div>
					  </li>
					</ul>
				  </div>
				  <div class="content-block">
					<div class="row">
					  <div class="col-100">
						<input onclick="return check();" style="z-index:9999" id="i_submit" type="submit" value="һ������" class="button button-big button-fill button-success"></div>
					</div>
				  </div>
			</form>
			%s
		</div>
	</div>
</div>