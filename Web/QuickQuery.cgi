<div class="page-group">
	<div class="page">
		<header class="bar bar-nav">
		<button class="button button-link button-nav pull-left"><span class="icon icon-left"></span><a href="index.cgi">����</a></button>
		<button class="button button-link button-nav pull-right"><a href="Comments.html">�²�</a>&nbsp;&nbsp;<span class="icon icon-message"></span></button>
		<h1 class="title">����ѧԺ �� ���ܳɼ���ѯ</h1>
		</header>
		<nav class="bar bar-tab">
		<a class="tab-item external active" href="index.cgi">
		<span class="icon icon-star"></span>
		<span class="tab-label">�ɼ���ѯ</span>
		</a>
		</nav>
		<div class="content">
			<div class="content-block" style="overflow: hidden">
				<p>
					����ѧ������ѯ�ɼ�����ʱֻ֧�ֹ���ѧԺ :)
				</p>
				<canvas id="i_canvas" style="position:absolute;top:0;left:0;right:0;bottom:0"></canvas>
				<form action="query.cgi?act=QuickQuery" method="post">
					<div class="list-block">
						<ul style="opacity: 0.75;z-index:9999">
							<li>
							<div class="item-content">
								<div class="item-media">
									<i class="icon icon-form-name"></i>
								</div>
								<div class="item-inner">
									<div class="item-title label">
										ѧ��
									</div>
									<div class="item-input">
										<textarea id="i_xhs" name="xh" type="text" placeholder="���ѧ�Ż�������(���5��)"></textarea>
									</div>
								</div>
							</div>
							</li>
						</ul>
					</div>
					<div class="content-block">
						<div class="row">
							<div class="col-100">
								<input onclick="return check();" style="z-index:9999" id="i_submit" type="submit" value="���ܲ�ѯ" class="button button-big button-fill button-success" />
							</div>
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="&lt; ��¼���ۺϲ�ѯ" class="button button-big button-fill external" href="index.cgi">&lt; ��¼���ۺϲ�ѯ</a>
							</div>
						</div>
					</div>
				</div>
			</form>
			<center><p id="login_query">��&nbsp;&nbsp;<big>%ld</big>&nbsp;&nbsp;λͬѧ������&nbsp;&nbsp;<big>%d</big>&nbsp;&nbsp;�β�ѯ</p></center>
		</div>
	</div>
</div>