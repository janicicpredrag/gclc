<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
<html>
	<head>
		<style> 
			font.name		{font: italic;}			
		</style>
	</head>
	<body>

	<xsl:for-each select="figure">
		<br/>	
		<font color="#0000ff">
		<xsl:text>Description of construction: </xsl:text>
		</font>
		<br/>	
		<br/>	
		<hr/>
		<br/>	
		<xsl:apply-templates />
		<hr/>
		<font color="#0000ff">
		<xsl:text>Transformed by GCLC-XML package (2006)</xsl:text>
		</font>
	</xsl:for-each>

	</body>
</html>
</xsl:template>

<!--**************introducing points**************-->

<xsl:template match="define">
	<font color="#DD0000">
	<xsl:text> Define: </xsl:text>
	</font>
	<br/>	
	<xsl:apply-templates /> 		
	<br/>	
</xsl:template>


<xsl:template match="fixed_point">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>
	<xsl:text> = point (</xsl:text>
	<xsl:value-of select="@x"/>,
	<xsl:value-of select="@y"/>)	
	<br/>	
</xsl:template>


<!--**************objects**************-->

<xsl:template match="dimensions">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;dimensions (</xsl:text>
	<font class="name"><xsl:value-of select="@width"/></font>mm, <font class="name"><xsl:value-of select="@height"/></font>mm)
	<br/>
</xsl:template>

<xsl:template match="coefficient">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;coefficient (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>
</xsl:template>

<xsl:template match="angle">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;angle (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>
</xsl:template>

<xsl:template match="point">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;point (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>
</xsl:template>

<xsl:template match="center">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;center (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>
</xsl:template>

<xsl:template match="segment">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;segment (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>) 
	<br/>
</xsl:template>


<xsl:template match="vector">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;vector (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="line">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;line (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>	
</xsl:template>

<xsl:template match="line_pp">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;line_pp (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="fixed_number">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="."/></font> = fixed number (<font class="name"><xsl:value-of select="@value"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="circle">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;circle (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>	
</xsl:template>

<xsl:template match="circle_cp">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;circle_cp (</xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>, <font class="name"><xsl:value-of select="point"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="arc">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;arc (</xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>, <font class="name"><xsl:value-of select="point"/></font>, 
<font class="name"><xsl:value-of select="angle"/></font>)
	<br/>
</xsl:template>


<!--**************constructions**************-->

<xsl:template match="construct">
	<font color="#00AAAA">
	<xsl:text>Construction steps:</xsl:text>
	</font>
	<br/>	
	<xsl:apply-templates /> 						
	<br/>	
</xsl:template>


<xsl:template match="new_point">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;new_point (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>	
</xsl:template>

<xsl:template match="intersection">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font> 
	<xsl:text> = intersection (</xsl:text>
	<font class="name"><xsl:value-of select="line[1]"/></font>, <font class="name"><xsl:value-of select="line[2]"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="intersection_cc">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	(<font class="name"><xsl:value-of select="new_point[1]"/></font>, <font class="name"><xsl:value-of select="new_point[2]"/></font>)  
	<xsl:text> = intersection_cc (</xsl:text>
	<font class="name"><xsl:value-of select="circle[1]"/></font>, <font class="name"><xsl:value-of select="circle[2]"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="intersection_cl">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	(<font class="name"><xsl:value-of select="new_point[1]"/></font>, <font class="name"><xsl:value-of select="new_point[2]"/></font>)  
	<xsl:text> = intersection_cl (</xsl:text>
	<font class="name"><xsl:value-of select="circle"/></font>, <font class="name"><xsl:value-of select="line"/></font>)
	<br/>	
</xsl:template>

<xsl:template match="midpoint">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = midpoint (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="foot">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/> </font> 
	<xsl:text> = foot (</xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>, <font class="name"><xsl:value-of select="line"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="random_point_on_line ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = random_point_on_line (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="translate">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = translate (</xsl:text>
	<xsl:text>vector (</xsl:text><font class="name"><xsl:value-of select="vector/point[1]"/></font>,
	<font class="name"><xsl:value-of select="vector/point[2]"/></font>), <font class="name"><xsl:value-of select="point"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="towards">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = towards (</xsl:text>
	<xsl:text>vector (</xsl:text><font class="name"><xsl:value-of select="vector/point[1]"/></font>,
	<font class="name"><xsl:value-of select="vector/point[2]"/></font>), <font class="name"><xsl:value-of select="coefficient"/></font>) 
	<br/>	
</xsl:template>

<xsl:template match="rotate ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = rotate (</xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>, <font class="name"><xsl:value-of select="angle"/></font>, 
	<font class="name"><xsl:value-of select="point"/></font>)	
	<br/>	
</xsl:template>

<xsl:template match="half_turn ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = half_turn (</xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>, <font class="name"><xsl:value-of select="point"/></font>)
	<br/>	
</xsl:template>

<xsl:template match="line_reflection ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = line_reflection (</xsl:text>
	<font class="name"><xsl:value-of select="line"/></font>, <font class="name"><xsl:value-of select="point"/></font>)	
	<br/>	
</xsl:template>

<xsl:template match="inversion ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> = inversion (</xsl:text>
	<font class="name"><xsl:value-of select="circle"/></font>, <font class="name"><xsl:value-of select="point"/></font>)
	<br/>	
</xsl:template>

<xsl:template match="new_line">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;new_line (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>	
</xsl:template>

<xsl:template match="ruler ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> = line (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>)		
	<br/>	
</xsl:template>

<xsl:template match="parallel ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> = parallel line </xsl:text>(<font class="name"><xsl:value-of select="point"/></font>, 
                           <font class="name"><xsl:value-of select="line"/></font>)
	<br/>	
</xsl:template>


<xsl:template match="perpendicular ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> = perpendicular (</xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>, <font class="name"><xsl:value-of select="line"/></font>)
	<br/>	
</xsl:template>

<xsl:template match="segment_bisector ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> = segment_bisector (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>)
	<br/>	
</xsl:template>

<xsl:template match="angle_bisector ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> = angle_bisector (</xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>, 
	 <font class="name"><xsl:value-of select="point[2]"/></font>)	
	<br/>	
</xsl:template>

<xsl:template match="new_circle">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;new_circle (</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>)
	<br/>	
</xsl:template>


<xsl:template match="compass ">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;</xsl:text>
	<font class="name"><xsl:value-of select="new_circle"/></font>  
	<xsl:text> = circle (</xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>, <font class="name"><xsl:value-of select="point"/></font>)
	<br/>	
</xsl:template>


<!--**************drawing**************-->

<xsl:template match="draw">			
	<font color="#0000DD">
	<xsl:text>Draw:</xsl:text>
	</font>
	<br/>	
	<xsl:apply-templates /> 						
	<br/>
</xsl:template>


<xsl:template match="draw[@dashed='no']">			
	<font color="#0000DD">
	<xsl:text> Draw (normal style): </xsl:text>
	</font>
	<br/>
	<xsl:apply-templates /> 						
	<br/>
</xsl:template>


<xsl:template match="draw[@dashed='yes']">			
	<font color="#0000DD">
	<xsl:text> Draw (dashed style): </xsl:text>
	</font>
	<br/>
	<xsl:apply-templates /> 						
	<br/>	
</xsl:template>

<xsl:template match="area">
	<xsl:text>&#160;&#160;&#160;&#160;Visible area: </xsl:text>
	left-bottom corner (<font class="name"><xsl:value-of select="@x1"/></font>,<font class="name"><xsl:value-of select="@y1"/></font>),
	right-top corner (<font class="name"><xsl:value-of select="@x2"/></font>, <font class="name"><xsl:value-of select="@y2"/></font>)
	<br/>	
</xsl:template>



<!--**************labeling**************-->


<xsl:template match="labels">
	<font color="#DD00DD">
	<xsl:text> Labels:</xsl:text>
	</font>
	<br/>	
	<xsl:apply-templates /> 						
	<br/>	
</xsl:template>

<xsl:template match="point_label">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> (direction = </xsl:text>
	<font class="name"><xsl:value-of select="@direction"/></font>
 degrees)

	<br/>
</xsl:template>

<xsl:template match="text_label">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> (direction = </xsl:text>
	<font class="name"><xsl:value-of select="@direction"/></font>
 degrees)
	<xsl:text>&#160;</xsl:text>
	<xsl:value-of select="text"/>
	<br/>
</xsl:template>


<!--**************conjectures***********-->

<xsl:template match="conjecture">
	<font color="#046000">
	<xsl:text> Conjectures: </xsl:text>
	</font>
	<br/>	
	<xsl:apply-templates /> 		
	<br/>
</xsl:template>

<xsl:template match="prove">
	<xsl:text>&#160;&#160;&#160;&#160;&#160;&#160;prove ( proof level = </xsl:text><xsl:value-of select="@proof_level"/>
	<xsl:text>, proof limit = </xsl:text><xsl:value-of select="@proof_limit"/>,
	<xsl:apply-templates />) 
	<br/>	
</xsl:template>

<xsl:template match="equality">
	<xsl:text> equal (</xsl:text>	 
	<xsl:apply-templates select="expression[1]"/>, <xsl:apply-templates select="expression[2]"/>)
</xsl:template>

<xsl:template match="expression">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="number">		
	<xsl:value-of select="."/>		
</xsl:template>

<xsl:template match="constant">		
	<xsl:value-of select="."/>		
</xsl:template>


<xsl:template match="sum">
	(<xsl:apply-templates  select="expression[1]"/>) + 
	(<xsl:apply-templates  select="expression[2]"/>)
</xsl:template>

<xsl:template match="mult">
	(<xsl:apply-templates  select="expression[1]"/>) * 
	(<xsl:apply-templates select="expression[2]"/>)
</xsl:template>

<xsl:template match="fraction">
	(<xsl:apply-templates  select="expression[1]"/>) / 
	(<xsl:apply-templates  select="expression[2]"/>)

</xsl:template>

<xsl:template match="segment_ratio">
	segment_ratio(
	<xsl:value-of select="segment[1]/point[1]"/>,  
	<xsl:value-of select="segment[1]/point[2]"/>; 
	<xsl:value-of select="segment[2]/point[1]"/>, 
	<xsl:value-of select="segment[2]/point[2]"/>
	)
</xsl:template>

<xsl:template match="signed_area3">
	s3(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>)
</xsl:template>

<xsl:template match="signed_area4">
	s4(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>,
	<xsl:value-of select="point[4]"/>)
</xsl:template>

<xsl:template match="pythagoras_difference3">
	p3(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>)

</xsl:template>

<xsl:template match="pythagoras_difference4">
	p4(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>,
	<xsl:value-of select="point[4]"/>)
</xsl:template>



</xsl:stylesheet>