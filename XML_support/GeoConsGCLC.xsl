<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
	<xsl:for-each select="figure">			
		<xsl:text>
% </xsl:text>
		<xsl:text>   
</xsl:text>	
		<xsl:text>% Description of construction: 
</xsl:text>		
		<xsl:text>% </xsl:text>
		<xsl:text>   
</xsl:text>	
		<xsl:text>   
</xsl:text>	
		<xsl:apply-templates />
	</xsl:for-each>

	
</xsl:template>

<!--**************introducing points**************-->

<xsl:template match="define">
	<xsl:apply-templates /> 		
	<xsl:text>   
</xsl:text>	
</xsl:template>


<xsl:template match="fixed_point">
	<xsl:text>point  </xsl:text>
	<xsl:value-of select="."/> 
	<xsl:text> </xsl:text>
	<xsl:value-of select="@x"/> 
	<xsl:text> </xsl:text>
	<xsl:value-of select="@y"/>	
	<xsl:text>   
</xsl:text>	
</xsl:template>

<!--**************objects**************-->


<xsl:template match="dimensions">
	<xsl:text>dim </xsl:text>
	<xsl:value-of select="@width"/><xsl:text> </xsl:text><xsl:value-of select="@height"/>  	
</xsl:template>

<xsl:template match="coefficient">
	<xsl:text>  </xsl:text>
	<xsl:value-of select="."/>  	
</xsl:template>

<xsl:template match="angle">
	<xsl:text> </xsl:text>
	<xsl:value-of select="."/>  	
</xsl:template>


<xsl:template match="point">
	<xsl:text> </xsl:text>
	<xsl:value-of select="."/> 
</xsl:template>

<xsl:template match="center">
	<xsl:text> </xsl:text>
	<xsl:value-of select="."/>  	
</xsl:template>


<xsl:template match="segment">
<xsl:choose>
<xsl:when test="../@dashed='yes'">
	<xsl:text>drawdashsegment </xsl:text>
</xsl:when>
<xsl:otherwise>
	<xsl:text>drawsegment </xsl:text>
</xsl:otherwise>
</xsl:choose>
	<xsl:value-of select="point[1]"/> 
	<xsl:text> </xsl:text>
 	<xsl:value-of select="point[2]"/>	
	<xsl:text>   
</xsl:text>
</xsl:template>

<xsl:template match="vector">
	<xsl:text>drawvector </xsl:text>
	<xsl:value-of select="point[1]"/> 
	<xsl:text> </xsl:text>
 	<xsl:value-of select="point[2]"/>	
	<xsl:text>   
</xsl:text>
</xsl:template>
	

<xsl:template match="line_pp">
	<xsl:choose>
	<xsl:when test="../@dashed='yes'">
	<xsl:text>drawdashline </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>drawline </xsl:text>
</xsl:otherwise>
</xsl:choose>
	<xsl:value-of select="point[1]"/> 
	<xsl:text> </xsl:text>
 	<xsl:value-of select="point[2]"/>	
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="line">
	<xsl:choose>
	<xsl:when test="../@dashed='yes'">
	<xsl:text>drawdashline </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>drawline </xsl:text>
</xsl:otherwise>
</xsl:choose>
	<xsl:value-of select="."/> 
	<xsl:text>   
</xsl:text>	
</xsl:template>


<xsl:template match="circle">
	<xsl:choose>
	<xsl:when test="../@dashed='yes'">
	<xsl:text>drawdashcircle </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>drawcircle </xsl:text>
</xsl:otherwise>
</xsl:choose>
<xsl:value-of select="."/> 	
</xsl:template>

<xsl:template match="circle_cp">
<xsl:choose>
	<xsl:when test="../@dashed='yes'">
	<xsl:text>drawdashcircle </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>drawcircle </xsl:text>
	</xsl:otherwise>
	</xsl:choose>
	<xsl:value-of select="center"/> 
	<xsl:text> </xsl:text>
 	<xsl:value-of select="point"/>	
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="arc">
	<xsl:choose>
	<xsl:when test="../@dashed='yes'">
	<xsl:text>drawdasharc </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>drawarc </xsl:text>
</xsl:otherwise>
</xsl:choose>
	<xsl:value-of select="center"/> 
	<xsl:text> </xsl:text>
 	<xsl:value-of select="point"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="angle"/> 
	<xsl:text>   
</xsl:text>

</xsl:template>


<!--**************constructions**************-->

<xsl:template match="construct">
	<xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>
	<xsl:apply-templates /> 						
	<xsl:text>   
</xsl:text>
	
</xsl:template>


<xsl:template match="new_point">
	<xsl:text> </xsl:text>
	<xsl:value-of select="."/> 
</xsl:template>

<xsl:template match="intersection">
	<xsl:text>intersec </xsl:text>
	<xsl:value-of select="new_point"/>
	<xsl:for-each select="line">
		<xsl:text> </xsl:text>
		<xsl:value-of select="."/>
	</xsl:for-each>	
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="intersection_cc">
	<xsl:text>intersec2 </xsl:text>
	<xsl:value-of select="new_point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="new_point[2]"/><xsl:text> </xsl:text>
	<xsl:value-of select="circle[1]"/><xsl:text> </xsl:text><xsl:value-of select="circle[2]"/>	
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="intersection_cl">
	<xsl:text>intersec2 </xsl:text>
	<xsl:value-of select="new_point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="new_point[2]"/><xsl:text> </xsl:text>
	<xsl:value-of select="circle"/><xsl:text> </xsl:text><xsl:value-of select="line"/>	
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="midpoint">
	<xsl:text>midpoint </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/><xsl:text> </xsl:text>	
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="foot">
	<xsl:text>foot </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/><xsl:text> </xsl:text>
	<xsl:value-of select="line"/>	
	<xsl:text>   
</xsl:text>

</xsl:template>

<xsl:template match="random_point_on_line ">
	<xsl:text>online </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/>	
	<xsl:text>   
</xsl:text>		
</xsl:template>

<xsl:template match="translate">
	<xsl:text>translate </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="vector/point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="vector/point[2]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/>		
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="towards">
	<xsl:text>towards </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="vector/point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="vector/point[2]"/><xsl:text> </xsl:text>
	<xsl:value-of select="coefficient"/>		
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="rotate">
	<xsl:text>rotate </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="center"/><xsl:text> </xsl:text>
	<xsl:value-of select="angle"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/>			
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="half_turn ">
	<xsl:text>sim </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="center"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/>	
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="line_reflection ">
	<xsl:text>sim </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="line"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/>	
	<xsl:text>   
</xsl:text>		
</xsl:template>

<xsl:template match="inversion ">
	<xsl:text>sim </xsl:text>
	<xsl:value-of select="new_point"/><xsl:text> </xsl:text>
	<xsl:value-of select="circle"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/>	
	<xsl:text>   
</xsl:text>		
</xsl:template>


<xsl:template match="new_line">
	<xsl:text> </xsl:text>
	<xsl:value-of select="."/> 	
</xsl:template>

<xsl:template match="ruler ">
	<xsl:text>line </xsl:text>
	<xsl:value-of select="new_line"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/><xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="parallel ">
	<xsl:text>parallel </xsl:text>
	<xsl:text> </xsl:text>
	<xsl:value-of select="new_point"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="line"/>	
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="perpendicular ">
	<xsl:text>perp </xsl:text>
	<xsl:value-of select="new_line"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="line"/>

	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="segment_bisector ">
	<xsl:text>med </xsl:text>
	<xsl:value-of select="new_line"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/><xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>
	
</xsl:template>

<xsl:template match="angle_bisector ">
	<xsl:text>bis </xsl:text>
	<xsl:value-of select="new_line"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[1]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/><xsl:text> </xsl:text>
	<xsl:value-of select="point[3]"/>	
	<xsl:text>   
</xsl:text>		
</xsl:template>


<xsl:template match="new_circle">
	<xsl:text> </xsl:text>
	<xsl:value-of select="."/> 	
</xsl:template>

<xsl:template match="compass ">
	<xsl:text>circle </xsl:text>
	<xsl:value-of select="new_circle"/><xsl:text> </xsl:text>
	<xsl:value-of select="center"/><xsl:text> </xsl:text>
	<xsl:value-of select="point"/>			
	<xsl:text>   
</xsl:text>	
</xsl:template>


<!--**************drawing**************-->

<xsl:template match="draw">			
	<xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>	
	<xsl:apply-templates /> 						
	<xsl:text>   
</xsl:text>

</xsl:template>


<xsl:template match="draw[@dashed='no']">			
	<xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>

	<xsl:apply-templates /> 						
	<xsl:text>   
</xsl:text>
</xsl:template>


<xsl:template match="draw[@dashed='yes']">			
	<xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>
	<xsl:apply-templates /> 						
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="area">
	<xsl:text>area </xsl:text>
<xsl:value-of select="@x1"/> <xsl:text> </xsl:text>
 <xsl:value-of select="@y1"/> <xsl:text> </xsl:text>
<xsl:value-of select="@x2"/> <xsl:text> </xsl:text>
<xsl:value-of select="@y2"/> <xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>	
</xsl:template>



<!--**************labeling**************-->


<xsl:template match="labels">
	<xsl:text> </xsl:text>
	<xsl:text>   
</xsl:text>	
	<xsl:apply-templates /> 						
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="point_label">
<xsl:choose>
<xsl:when test="@direction &gt; 0">
	<xsl:choose>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 22.5">
		<xsl:text>cmark_r </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 67.5">
	<xsl:text>cmark_rt </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 112.5">
	<xsl:text>cmark_t </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 157.5">
	<xsl:text>cmark_lt </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 202.5">
	<xsl:text>cmark_l </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 247.5">
	<xsl:text>cmark_lb </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 292.5">
	<xsl:text>cmark_b </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 337.5">
	<xsl:text>cmark_rb </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 361">
	<xsl:text>cmark_r </xsl:text>
	</xsl:when>

	<xsl:otherwise>
	<xsl:text>cmark </xsl:text>
	</xsl:otherwise>
	</xsl:choose>
</xsl:when>

<xsl:otherwise>
	<xsl:choose>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 22.5">
		<xsl:text>cmark_r </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 67.5">
	<xsl:text>cmark_rt </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 112.5">
	<xsl:text>cmark_t </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 157.5">
	<xsl:text>cmark_lt </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 202.5">
	<xsl:text>cmark_l </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 247.5">
	<xsl:text>cmark_lb </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 292.5">
	<xsl:text>cmark_b </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 337.5">
	<xsl:text>cmark_rb </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 361">
	<xsl:text>cmark_r </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>cmark </xsl:text>
	</xsl:otherwise>
	</xsl:choose>

</xsl:otherwise>
</xsl:choose>

	<xsl:apply-templates /> 
	<xsl:text>   
</xsl:text>
</xsl:template>


<xsl:template match="text_label">
<xsl:choose>
<xsl:when test="@direction &gt; 0">
	<xsl:choose>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 22.5">
		<xsl:text>printat_r </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 67.5">
	<xsl:text>printat_rt </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 112.5">
	<xsl:text>printat_t </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 157.5">
	<xsl:text>printat_lt </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 202.5">
	<xsl:text>printat_l </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 247.5">
	<xsl:text>printat_lb </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 292.5">
	<xsl:text>printat_b </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 337.5">
	<xsl:text>printat_rb </xsl:text>
	</xsl:when>
	<xsl:when test="(@direction mod 360) &lt; 360">
	<xsl:text>printat_r </xsl:text>
	</xsl:when>

	<xsl:otherwise>
	<xsl:text>printat</xsl:text>
	</xsl:otherwise>
	</xsl:choose>
</xsl:when>

<xsl:otherwise>
	<xsl:choose>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 22.5">
		<xsl:text>printat_r </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 67.5">
	<xsl:text>printat_rt </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 112.5">
	<xsl:text>printat_t </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 157.5">
	<xsl:text>printat_lt </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 202.5">
	<xsl:text>printat_l </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 247.5">
	<xsl:text>printat_lb </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 292.5">
	<xsl:text>printat_b </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 337.5">
	<xsl:text>printat_rb </xsl:text>
	</xsl:when>
	<xsl:when test="(-((-@direction) mod 360)+360) &lt; 360">
	<xsl:text>printat_r </xsl:text>
	</xsl:when>
	<xsl:otherwise>
	<xsl:text>printat</xsl:text>
	</xsl:otherwise>
	</xsl:choose>

</xsl:otherwise>
</xsl:choose>
	<xsl:apply-templates /> 
	<xsl:text>   
</xsl:text>

</xsl:template>

<xsl:template match="text">
	<xsl:text> {</xsl:text>
	<xsl:value-of select="."/> 
	<xsl:text>} </xsl:text>	
</xsl:template>

<!--**************conjectures***********-->

<xsl:template match="conjecture">
	<xsl:apply-templates /> 		
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="prove">
	<xsl:text>prooflevel </xsl:text><xsl:value-of select="@proof_level"/>
	<xsl:text>   
</xsl:text>
	<xsl:apply-templates /> 		
</xsl:template>

	

<xsl:template match="equality">
	<xsl:text>prove { equal </xsl:text>	 
	<xsl:apply-templates />
<xsl:text>}</xsl:text>
	<xsl:text>   
</xsl:text>	
</xsl:template>

<xsl:template match="expression">{ <xsl:apply-templates /> } </xsl:template>

<xsl:template match="number">
	<xsl:value-of select="."/>
	<xsl:text> </xsl:text>	
</xsl:template>

<xsl:template match="constant">
	<xsl:value-of select="."/>
	<xsl:text> </xsl:text>	
</xsl:template>

<xsl:template match="sum">
	<xsl:text>sum </xsl:text>
		<xsl:apply-templates />
</xsl:template>

<xsl:template match="mult">
	<xsl:text>mult </xsl:text>
	<xsl:apply-templates />
</xsl:template>

<xsl:template match="fraction">
	<xsl:text>fraction </xsl:text>	
</xsl:template>

<xsl:template match="segment_ratio">
	<xsl:text>sratio </xsl:text>
	<xsl:value-of select="segment[1]/point[1]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="segment[1]/point[2]"/>
	<xsl:text> </xsl:text>	
<xsl:value-of select="segment[2]/point[1]"/>
	<xsl:text> </xsl:text>	
<xsl:value-of select="segment[2]/point[2]"/>
	<xsl:text> </xsl:text>	
</xsl:template>

<xsl:template match="signed_area3">
	<xsl:text>signed_area3 </xsl:text>
	<xsl:value-of select="point[1]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/>
	<xsl:text> </xsl:text>	
	<xsl:value-of select="point[3]"/>
</xsl:template>

<xsl:template match="signed_area4">
	<xsl:text>signed_area4 </xsl:text>
	<xsl:value-of select="point[1]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/>
	<xsl:text> </xsl:text>	
	<xsl:value-of select="point[3]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point[4]"/>
</xsl:template>

<xsl:template match="pythagoras_difference3">
	<xsl:text>pythagoras_difference3 </xsl:text>
	<xsl:value-of select="point[1]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/>
	<xsl:text> </xsl:text>	
<xsl:value-of select="point[3]"/>
</xsl:template>

<xsl:template match="pythagoras_difference4">
	<xsl:text>pythagoras_difference4 </xsl:text>
	<xsl:value-of select="point[1]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point[2]"/>
	<xsl:text> </xsl:text>	
	<xsl:value-of select="point[3]"/>
	<xsl:text> </xsl:text>
	<xsl:value-of select="point[4]"/>
</xsl:template>
</xsl:stylesheet>