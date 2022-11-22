// https://observablehq.com/@louisjuin/draft-d3js-donut-leisure-business@30
function _1(md){return(
md`# Draft D3js - donut Leisure-Business`
)}

function _d3(require){return(
require("d3@5")
)}

function _pie(d3){return(
d3.pie()
    .padAngle(0.005)
    .sort(null)
    .value(d => d.value)
)}

function _height(width){return(
Math.min(width, 500)
)}

function _arc(width,height,d3)
{
  const radius = Math.min(width, height) / 2;
  return d3.arc().innerRadius(radius * 0.67).outerRadius(radius - 1);
}


async function _data(d3,FileAttachment){return(
d3.csvParse(await FileAttachment("leasure-business-csv-3.csv").text(), d3.autoType)
)}

function _color(d3,data){return(
d3.scaleOrdinal()
    .domain(data.map(d => d.name))
    .range(d3.quantize(t => d3.interpolateSpectral(t * 0.8 + 0.1), data.length).reverse())
)}

function _chart(pie,data,d3,width,height,color,arc)
{
  const arcs = pie(data);

  const svg = d3.create("svg")
      .attr("viewBox", [-width / 2, -height / 2, width, height]);

  svg.selectAll("path")
    .data(arcs)
    .join("path")
      .attr("fill", d => color(d.data.name))
      .attr("d", arc)
    .append("title")
      .text(d => `${d.data.name + 'matches'}: ${d.data.value.toLocaleString()}`);

  svg.append("g")
      .attr("font-family", "sans-serif")
      .attr("font-size", 12)
      .attr("text-anchor", "middle")
    .selectAll("text")
    .data(arcs)
    .join("text")
      .attr("transform", d => `translate(${arc.centroid(d)})`)
      .call(text => text.append("tspan")
          .attr("y", "-0.4em")
          .attr("font-weight", "bold")
          .text(d => d.data.name ))
      .call(text => text.filter(d => (d.endAngle - d.startAngle) > 0.25).append("tspan")
          .attr("x", 0)
          .attr("y", "0.7em")
          .attr("fill-opacity", 0.7)
          .text(d => d.data.value.toLocaleString()));

  return svg.node();
}


export default function define(runtime, observer) {
  const main = runtime.module();
  function toString() { return this.url; }
  const fileAttachments = new Map([
    ["leasure-business-csv-3.csv", {url: new URL("./files/donutDataBase.csv", import.meta.url), mimeType: "text/csv", toString}]
  ]);
  main.builtin("FileAttachment", runtime.fileAttachments(name => fileAttachments.get(name)));
  main.variable(observer()).define(["md"], _1);
  main.variable(observer("d3")).define("d3", ["require"], _d3);
  main.variable(observer("pie")).define("pie", ["d3"], _pie);
  main.variable(observer("height")).define("height", ["width"], _height);
  main.variable(observer("arc")).define("arc", ["width","height","d3"], _arc);
  main.variable(observer("data")).define("data", ["d3","FileAttachment"], _data);
  main.variable(observer("color")).define("color", ["d3","data"], _color);
  main.variable(observer("chart")).define("chart", ["pie","data","d3","width","height","color","arc"], _chart);
  return main;
}
