package com.jesen.openglnative

import android.content.Context
import android.graphics.Color
import android.provider.CalendarContract.Colors
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.RadioButton
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.jesen.openglnative.databinding.SampleItemLayoutBinding

class MainRecyclerAdapter(val context: Context, val titles: List<String>) :
    RecyclerView.Adapter<MainRecyclerAdapter.MainViewHolder>(),View.OnClickListener {

    private var mSelectIndex = 0
    private var mOnItemClickListener: OnItemClickListener? = null

    class MainViewHolder(private val itemView: View) : RecyclerView.ViewHolder(itemView) {
        val mRadioButton: RadioButton = itemView.findViewById(R.id.radio_btn);
        val mTitle: TextView = itemView.findViewById(R.id.item_title)
    }

    fun setSelectIndex(index:Int){
        mSelectIndex = index
    }

    fun getSelectIndex():Int = mSelectIndex

    fun addOnItemClickListener(listener: OnItemClickListener){
        mOnItemClickListener = listener
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MainViewHolder {
        val binding: SampleItemLayoutBinding =
            SampleItemLayoutBinding.inflate(LayoutInflater.from(parent.context))
        binding.root.setOnClickListener(this)
        return MainViewHolder(binding.root)
    }

    override fun getItemCount(): Int =titles.size

    override fun onBindViewHolder(holder: MainViewHolder, position: Int) {
        holder.mTitle.setText(titles[position])
        if (position == mSelectIndex){
            holder.mRadioButton.isChecked = true
            holder.mTitle.setTextColor(context.resources.getColor(R.color.purple_200))
        }else{
            holder.mRadioButton.isChecked = false
            holder.mTitle.text = titles[position]
            holder.mTitle.setTextColor(Color.GRAY)
        }
        holder.itemView.tag = position
    }

    override fun onClick(v: View) {
        mOnItemClickListener?.onItemClick(v,v.tag as Int)
    }
}